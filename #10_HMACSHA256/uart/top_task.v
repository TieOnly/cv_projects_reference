`timescale 1ns / 1ps

module TOP(
    	Clk                     ,
    	Rst_n                   ,   
		Rx                      ,    
    	Tx                      ,
		RxData					,
		TxEnable				,
		LED,
		SW,
	);

/////////////////////////////////////////////////////////////////////////////////////////
input     		Clk             ; // Clock
input           Rst_n           ; // Reset
input           Rx              ; // RS232 RX line.
input			TxEnable		;
output          Tx              ; // RS232 TX line.
output [7:0]    RxData          ; // Received data
output [7:0]	LED;
reg    [7:0]	LED = 8'hff; 
input  [9:0]	SW;
/////////////////////////////////////////////////////////////////////////////////////////
wire [7:0]    	TxData     		; // Data to transmit.
wire          	RxDone          ; // Reception completed. Data is valid.
wire          	TxDone          ; // Trnasmission completed. Data sent.
wire            tick			; // Baud rate clock
wire 			RxEn			;
wire [3:0]      NBits    		;
wire [15:0]    	BaudRate        ; //328; 162 etc... (Read comment in baud rate generator file)
/////////////////////////////////////////////////////////////////////////////////////////
assign 		RxEn = 1'b1	;
assign 		BaudRate = 16'd325; 	//baud rate set to 9600 for the HC-06 bluetooth module. Why 325? (Read comment in baud rate generator file)
assign 		NBits = 4'b1000	;	//We send/receive 8 bits
/////////////////////////////////////////////////////////////////////////////////////////


//Make connections between Rx module and TOP inputs and outputs and the other modules
UART_rs232_rx I_RS232RX(
    	.Clk(Clk)             	,
   	.Rst_n(Rst_n)         	,
    	.RxEn(RxEn)           	,
    	.RxData(RxData)       	,
    	.RxDone(RxDone)       	,
    	.Rx(Rx)               	,
    	.Tick(tick)           	,
    	.NBits(NBits)
    );


//Make connections between Tx module and TOP inputs and outputs and the other modules
reg			TxEn = 1'b0;
reg [23:0]	cnt;
parameter   TRANS_IDLE  = 0, 
			TRANS_LOAD  = 1,  
			TRANS_WRITE = 2;
reg	[1:0]	Trans_State;
reg			Trans_SW_State;
parameter	MSG_DELAY        = 32'd50_000_000;
parameter	CHAR_DELAY       = 32'd1_000;
reg [31:0]	pause_msg;
reg [31:0]	pause_char;
reg [(70*8)-1:0]	byte_str = "Hello World?    Hello World?   \n";
reg [32:0]			byte_index;
reg [32:0]			byte_len = 65;
reg [7:0]		tx_data = 8'd49;
assign			TxData = tx_data;

UART_rs232_tx I_RS232TX(
   	.Clk(Clk)            	,
    .Rst_n(Rst_n)         	,
    .TxEn(TxEn)           	,
    .TxData(TxData)      	,
   	.TxDone(TxDone)      	,
   	.Tx(Tx)               	,
   	.Tick(tick)           	,
   	.NBits(NBits)
);

reg byte_str_we;
reg [(64*8)-1:0] byte_str_tmp;

task write_buff ( input [(64*8)-1:0] data );
	begin
		byte_str_tmp = data;
	end
endtask

task write_buff_hex( input [255:0] data );
	begin: write_buff_hex_bl
		reg [31:0] i;
		reg [7:0] c;
		reg [31:0] id_d;
		reg [31:0] id_b;
		
		for (i = 256; i >= 4; i = i - 4) begin
			id_d = i-1;
			id_b = 2*i - 1;
			c = data[id_d -:4];
			if (c > 8'd9) c = c + 8'd87;
			else c = c + 8'd48;
			byte_str_tmp[id_b -:8] = c;
		end
	end
endtask

reg [32:0]	count = 0;
task toggle_led_0;
	begin
		LED[0] <= !LED[0];
		byte_str_we = 1;
		byte_str[(65*8)-1:8] = byte_str_tmp;
		byte_str[7:0] = "\n";
		byte_str_we = 0;
	end
endtask

always @ (posedge Clk) begin

	case(Trans_State)
		TRANS_IDLE: begin
			TxEn = 0;
			if (TxEnable) begin
				pause_msg <= pause_msg + 32'd1;
				if (pause_msg > MSG_DELAY) begin
					byte_index	= (byte_len*8)-1;
					pause_msg <= 0;
					Trans_State = TRANS_LOAD;
				end			
			end
		end
	
		TRANS_LOAD: begin
			TxEn = 0;
			pause_char <= pause_char + 32'd1;
			if (pause_char > CHAR_DELAY) begin
				pause_char <= 0;
				if (!TxDone) begin				//Wait TxDone clear
					tx_data <= byte_str[byte_index -:8];
					//LED[1] = !LED[1];			
					Trans_State = TRANS_WRITE;
				end
			end
		end
	
		TRANS_WRITE: begin
			TxEn = 1;
			
			if (TxDone) begin					//Done transmit 1 char
				//LED[2] = !LED[2];
				if (byte_index<8) begin 
					Trans_State = TRANS_IDLE;
					toggle_led_0();
				end else begin
					byte_index <= byte_index - 8;
					Trans_State = TRANS_LOAD;
				end
			end
		end
		
		default;
	endcase

end

// always @ (posedge Clk) begin

	// if (SW[0]) begin
		// write_buff("123\n");
	// end else begin
		// write_buff("Hello\n");
	// end
	
// end

//Make connections between tick generator module and TOP inputs and outputs and the other modules
UART_BaudRate_generator I_BAUDGEN(
    	.Clk(Clk)               ,
    	.Rst_n(Rst_n)           ,
    	.Tick(tick)             ,
    	.BaudRate(BaudRate)
    );

reg sha_rst;
reg sha_byte_rdy;
reg sha_byte_stop;
reg  [7:0] sha_data_in;
wire sha_overflow_err;
wire sha_padd_done;
wire sha_flag_0_15;
wire [31:0] sha_K;
wire [6:0] sha_counter_iteration;
wire [255:0] sha_Hash_Digest;

top_sha sha_256(
	.clk(Clk),
	.rst(sha_rst),
	.byte_rdy(sha_byte_rdy),
	.byte_stop(sha_byte_stop),
	.data_in(sha_data_in),
	.overflow_err(sha_overflow_err),
	.padding_done(sha_padd_done),
	.K(sha_K),
	.flag_0_15(sha_flag_0_15),
	.counter_iteration(sha_counter_iteration),
	.Hash_Digest(sha_Hash_Digest)
);


reg [31:0]	pause_byte_rd;
reg [31:0]	pause_sha_rst;
reg [31:0]	pause_sha_byte_rd;
reg [31:0]	pause_sha_wait_digest;

parameter   SHA_IDLE  = 0, 
			SHA_LOAD  = 1,  
			SHA_WRITE = 2,
			SHA_WAIT = 3;
reg	[1:0]	Sha_State = 0;

reg [7:0]	sha_byte_data [2:0];
reg [31:0]	sha_cnt_byte;

initial begin
	Trans_State = TRANS_IDLE;
	LED = 8'hff;
	byte_index = (byte_len*8)-1;
	pause_msg = 0;
	pause_char = 0;
	byte_str_we = 0;
	pause_byte_rd = 0;
	pause_sha_rst = 0;
	pause_sha_byte_rd = 0;
	sha_cnt_byte = 0;
	write_buff("Hello");
	sha_byte_data[0] = "D";
	sha_byte_data[1] = "T";
	sha_byte_data[2] = "1";	
end

always @ (posedge Clk) begin

	case(Sha_State)
	
		SHA_IDLE: begin
			sha_rst = 0;
			sha_byte_stop = 0;
			pause_sha_rst = pause_sha_rst + 32'd1;
			if (pause_sha_rst > 32'd100_000_000) begin
				pause_sha_rst = 0;
				sha_rst = 1;
				sha_data_in = sha_byte_data[sha_cnt_byte];
				sha_byte_rdy = 1;
				Sha_State = SHA_LOAD;
				LED[1] = !LED[1];
			end
		end
		SHA_LOAD: begin	
			sha_cnt_byte = sha_cnt_byte + 32'd1;
			if (sha_cnt_byte == 3) begin
				sha_cnt_byte = 0;
				sha_byte_rdy = 0;
				sha_byte_stop = 1;
				LED[2] = !LED[2];
				Sha_State = SHA_WRITE;
			end else begin
				sha_data_in = sha_byte_data[sha_cnt_byte];
			end
		end
		SHA_WRITE: begin
			if (sha_padd_done) begin
				LED[3] = !LED[3];
				Sha_State = SHA_WAIT;
			end
			//if (sha_overflow_err) LED[5] = !LED[5];
		end
		SHA_WAIT: begin
			if (sha_counter_iteration == 64) begin 
				LED[4] = !LED[4];
				pause_sha_wait_digest = pause_sha_wait_digest + 32'd1;
				if (pause_sha_wait_digest > 32'd1) begin
					pause_sha_wait_digest = 0;
					write_buff_hex(sha_Hash_Digest);
					if (sha_Hash_Digest[255:248] == 8'ha8) LED[5] = !LED[5];
					Sha_State = SHA_IDLE;
				end
			end
		end
		
	endcase

end

endmodule