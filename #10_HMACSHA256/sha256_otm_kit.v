`timescale 1ns / 1ps

module SHA256_KIT(

	input         		clk,
	input				reset,
	output reg [7:0]	LED,
	
	input rx_done,
	input [7:0] rx_data,
	input tick,
	input [15:0] baudrate,
	output wire Tx_Pin,
	input TxEnable
	
);

	reg H1_rst, H1_byte_rdy, H1_byte_stop;
	reg [7:0] H1_data_in;
	wire H1_block_full;
	wire [255:0] H1_hash;
	wire H1_done_hash;
	wire done_digest;
	wire [31:0] h_out;
	
	top_sha H1(
		.clk(clk),
		.rst(H1_rst),
		.byte_rdy(H1_byte_rdy),
		.byte_stop(H1_byte_stop),
		.data_in(H1_data_in),
		.block_full(H1_block_full),
		.Hash_Digest(H1_hash),
		.done_hash(H1_done_hash),
		.done_digest(done_digest),
		.h_out(h_out)
	);
	
	wire Rst_n  ; assign Rst_n = !reset;
	reg			TxEn = 1'b0;
	wire		TxDone;
	wire [7:0]	TxData;
	wire [3:0]  NBits; assign NBits = 4'b1000;
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
		.Clk(clk)            	,
		.Rst_n(Rst_n)         	,
		.TxEn(TxEn)           	,
		.TxData(TxData)      	,
		.TxDone(TxDone)      	,
		.Tx(Tx_Pin)             ,
		.Tick(tick)           	,
		.NBits(NBits)
	);

	reg byte_str_we;
	reg [(64*8)-1:0] byte_str_tmp = 512'd0;
	reg [(32*8)-1:0] byte_str_tmp_hex = 256'd0;

	task write_buff ( input [(64*8)-1:0] data );
		begin
			byte_str_tmp = data;
		end
	endtask

	task write_buff_hex( input wire [255:0] data );
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
	
	task write_buff_as_hex();
		begin: write_buff_as_hex
			reg [31:0] i;
			reg [7:0] c;
			reg [31:0] id_d;
			reg [31:0] id_b;
			
			for (i = 256; i >= 4; i = i - 4) begin
				id_d = i-1;
				id_b = 2*i - 1;
				c = byte_str_tmp_hex[id_d -:4];
				if (c > 8'd9) c = c + 8'd87;
				else c = c + 8'd48;
				byte_str_tmp[id_b -:8] = c;
			end
		end
	endtask

	reg [32:0]	count = 0;
	task toggle_led_0;
		begin
			byte_str_we = 1;
			byte_str[(65*8)-1:8] = byte_str_tmp;
			byte_str[7:0] = "\n";
			byte_str_we = 0;
		end
	endtask

	always @ (posedge clk) begin

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
						Trans_State = TRANS_WRITE;
					end
				end
			end
		
			TRANS_WRITE: begin
				TxEn = 1;
				
				if (TxDone) begin					//Done transmit 1 char
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

	reg [3:0] stage_sha;
	reg [3:0] H1_stage;
	reg H1_push;
	reg [7:0] data_byte;
	reg rx_new;
	reg rx_new_temp;
	reg done;
	reg [7:0] debug_byte;
	reg [255:0] debug_str;

	
initial begin
	stage_sha = 0;
	H1_stage = 0;
	H1_push = 0;
	done = 0;
	data_byte = 8'd0;
	
	H1_rst = 0;
	H1_byte_rdy = 0;
	H1_byte_stop = 0;
	
	Trans_State = TRANS_IDLE;
	pause_msg = 0;
	pause_char = 0;
	debug_byte = 0;
	debug_str = 256'hc6e693d0b35805080632bc2469e1154a8d1072a86557778c27a01329630f8917;
	
	rx_new = 1;
	rx_new_temp = 0;
	
	LED[7:0] = 8'hff;
	write_buff("Hello");
end

always @ (posedge clk) begin
	
	if (reset) begin
		H1_push = 0;
		debug_byte = 8'h00;
	end else begin
		case(stage_sha)
		
		0: begin 
			if (rx_done) begin
				if (rx_new) begin
					data_byte = rx_data;
					LED[3] = !LED[3];
					
					if (!H1_block_full) begin
						LED[0] = !LED[0];
						rx_new = 0;
						H1_push = 1;
					end else LED[2] = !LED[2];
					
					if (rx_data == 8'h0a) begin
						rx_new = 1;
						stage_sha = 1;
					end
					debug_byte[0] = !debug_byte[0];
				end else H1_push = 0;
			end else begin
				rx_new = 1;
			end
		end
		
		1: begin
			H1_push = 0;
			if (H1_done_hash) begin
				byte_str_tmp_hex = H1_hash;
				write_buff_as_hex();
				debug_byte[1] = !debug_byte[1];
				stage_sha = 2;
			end
		end
		
		2: begin
			if (!rx_done) begin
				stage_sha = 0;
			end
		end
		
		endcase
	end
	
end

always @ (posedge clk) begin

	if (reset) begin
		H1_rst = 0;
		H1_byte_rdy = 0;
		H1_byte_stop = 0;
		H1_data_in = 0;
		H1_stage = 0;
	end else begin
	
		case(H1_stage)
		
		0: begin
			H1_rst = 0;
			if (!rx_done) begin
				H1_byte_rdy = 0;
				H1_byte_stop = 0;
				H1_rst = 1;
				H1_stage = 1;
			end
		end
		
		1: begin 
			if (H1_push) begin
				if (data_byte != 8'h0a) begin	//LF
					H1_data_in = data_byte;
					H1_byte_rdy = 1;
				end else begin
					H1_byte_rdy = 0;
					H1_byte_stop = 1;
					H1_stage = 2;
				end
				LED[1] = !LED[1];
			end else begin
				H1_byte_rdy = 0;
			end
		end
		
		2: begin
			if (H1_done_hash) begin
				LED[4] = !LED[4];
				H1_stage = 0;
			end
		end
		
		endcase
	end
end

endmodule