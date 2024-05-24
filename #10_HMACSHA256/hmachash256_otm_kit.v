`timescale 1ns / 1ps

module HMACSHA256(

	input         		clk,
	input				reset,
	output reg			done,
	output reg [7:0]	LED,
	input [9:0]			SW,
	
	output reg [3:0] stage_hmac,
	output reg ready_read_mes,
	output reg [7:0] data_byte,
	output reg [3:0] H1_stage,
	
	input Rx_Pin,
	output Tx_Pin,
	output reg [7:0] RxData
);
	reg [255:0] fst_hash;
	reg H1_rst, H1_byte_rdy, H1_byte_stop;
	reg [7:0] H1_data_in;
	wire H1_block_full;
	wire [255:0] H1_hash;
	wire H1_done_hash;
	
	top_sha H1(
		.clk(clk),
		.rst(H1_rst),
		.byte_rdy(H1_byte_rdy),
		.byte_stop(H1_byte_stop),
		.data_in(H1_data_in),
		.block_full(H1_block_full),
		.hash(H1_hash),
		.done_hash(H1_done_hash)
	);
	
	//------------------------UART--------------------------//
	reg Rst_n;

	wire [15:0] BaudRate; assign BaudRate = 16'd325;
	wire tick;
	UART_BaudRate_generator I_BAUDGEN(
    	.Clk(clk)               ,
    	.Rst_n(Rst_n)           ,
    	.Tick(tick)             ,
    	.BaudRate(BaudRate)
    );

	// wire RxDone;
	// wire [7:0] RxData;
	// wire RxEn; assign RxEn = 1;
	// UART_rs232_rx RX_IN(
    	// .Clk(clk)             	,
		// .Rst_n(Rst_n)         	,
    	// .RxEn(RxEn)           	,
    	// .RxData(RxData)       	,
    	// .RxDone(RxDone)       	,
    	// .Rx(Rx_Pin)             ,
    	// .Tick(tick)           	,
    	// .NBits(NBits)
    // );
	
	// reg [7:0] tx_data;
	// wire TxDone;
	// wire [3:0] NBits; assign NBits = 4'b1000;
	// UART_rs232_tx TX_OUT(
		// .Clk(clk)            	,
		// .Rst_n(Rst_n)         	,
		// .TxEn(SW[0])        	,
		// .TxData(tx_data)      	,
		// .TxDone(TxDone)      	,
		// .Tx(Tx_Pin)             ,
		// .Tick(tick)           	,
		// .NBits(NBits)
    // );
	
	
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
	
	// reg [7:0] RxData;
	wire RxEn; assign RxEn = 1;

	parameter  IDLE = 1'b0, READ = 1'b1;
	reg [1:0] State, Next;
	reg  read_enable = 1'b0;
	reg  start_bit = 1'b1;
	reg [4:0]Bit = 5'b00000;
	reg [3:0] counter = 4'b0000;
	reg [7:0] Read_data= 8'b00000000;	
	//------------------------------------------------------//
	
	
	
	
	
	
	
	
	
	//reg [3:0] stage_hmac;
	reg [3:0] stage_hmac_last;
	reg [512:0] buff;
	reg [31:0] buff_cnt;
	wire rx_done;
	reg [7:0] rx_data;
	reg rx_new;
	reg rx_new_temp;
	reg tx_done;
	// reg [255:0] fst_hash;
	// reg ready_read_mes;
	// reg [7:0] data_byte;
	
	// reg [2:0] H1_stage;
	reg H1_push;
	reg pause_H1_rst;
	reg [31:0] H1_inp_idx; 
	reg one_rst;
	// reg this_done_hash;
	
initial begin
	done = 1'b0;
	fst_hash= 256'd0;
	stage_hmac = 0;
	stage_hmac_last = 0;
	buff = 512'd0;
	buff_cnt = 32'd511;
	ready_read_mes = 0;
	data_byte = 8'd0;
	
	H1_stage = 0;
	pause_H1_rst = 1;
	H1_push = 0;
	
	rx_new = 1;
	rx_new_temp = 0;
	one_rst = 1;
end

	
always @ (posedge clk) begin
	if (reset) begin
		rx_new = 1;
		Rst_n = 0;
		rx_data = 0;
	end else begin
		Rst_n = 1;
		rx_data = RxData;
		tx_done = TxDone;
		
		if (stage_hmac == 0 || stage_hmac == 3) begin
			if (!H1_block_full && rx_new_temp) rx_new = 0;
		end else begin
			rx_new = 1;
		end
	end
end

always @ (posedge clk) begin
	
	if (reset) begin
		LED[7:0] = 8'hff;
		stage_hmac = 10;
		stage_hmac_last = 0;
		H1_push = 0;
		done = 0;
	end else begin
	
		case(stage_hmac)
		
		0: begin 
			if(rx_new) begin
				rx_new_temp = rx_new;
				data_byte = rx_data;
				
				if (!H1_block_full) begin
					H1_push = 1;
					stage_hmac = 10;
					stage_hmac_last = 0;
				end
				
				if (rx_data == 8'h0a) stage_hmac = 1;
				else begin
					buff[buff_cnt -: 8] = rx_data;
					if (buff_cnt > 7) 
						buff_cnt = buff_cnt - 32'd8;
					else if (buff_cnt < 7) 
						buff_cnt = 32'd1;
					else buff_cnt = 32'd0;
				end
				
			end else begin
				H1_push = 0;
				rx_new_temp = 0;
			end
		end
		
		1: begin
			LED[0] = 0;
			if (buff_cnt != 1) begin	//K <= B
				buff_cnt = 32'd511;
				// H1_inp_idx = 32'd511;
				H1_push = 0;
				stage_hmac = 2;
			end else begin				//K > B
				if (H1_done_hash) begin
					buff[511 -: 256] = H1_hash;
					buff[255:0] = 256'd0;
					buff_cnt = 32'd511;
					// H1_inp_idx = 32'd511;
					H1_push = 0;
					stage_hmac = 2;
				end
			end
		end
		
		2: begin	//Wait hash push Key + Ipad
			H1_push = 1;
			if (H1_inp_idx < 8) begin
				stage_hmac = 3;
				H1_push = 0;
			end
		end
		
		3: begin	//Push Data Text
			if(rx_new) begin
				rx_new_temp = rx_new;
				data_byte = rx_data;
				if (!H1_block_full) begin
					H1_push = 1;
					stage_hmac = 10;
					stage_hmac_last = 3;
				end
				
				if (rx_data == 8'h0a) stage_hmac = 4;
			end else begin 
				H1_push = 0;
				rx_new_temp = 0;
			end
		end
	
		4: begin	//Wait hash done for fst hash
			if (H1_done_hash) begin
				fst_hash = H1_hash;
				buff_cnt = 32'd511;
				// H1_inp_idx = 32'd511;
				H1_push = 1;
				stage_hmac = 5;
				LED[1] = 0;
			end
		end
		
		5: begin	//Reset hash 
			//H1_push = 1;
			if (H1_inp_idx == buff_cnt) begin
				stage_hmac = 6;
			end
		end
		
		6: begin	//Wait hash Key + Opad + Fsthash
			if (H1_done_hash) begin
				fst_hash = H1_hash;
				H1_push = 0;
				done = 1;
				stage_hmac = 10;
				stage_hmac_last = 0;
				LED[2] = 0;
			end
		end
		
		
		
		
		10: begin
			case(State)
			0: begin
				if (!Rx_Pin && RxEn) begin
					start_bit = 1'b1;
					counter = 4'b0000;
					State = 1;
				end
			end
			
			1: begin
				if (tick) begin
					counter = counter+1;
	
					if ((counter == 4'b1000) & (start_bit)) begin
						start_bit = 1'b0;
						counter = 4'b0000;
					end

					if ((counter == 4'b1111) & (!start_bit) & (Bit < NBits)) begin
						Bit = Bit+1;
						Read_data = {Rx_Pin,Read_data[7:1]};
						counter = 4'b0000;
					end
					
					if ((counter == 4'b1111) & (Bit == NBits)  & (Rx_Pin)) begin
						Bit = 4'b0000;
						RxData[7:0] = Read_data[7:0];
						counter = 4'b0000;
						start_bit = 1'b1;
						State = 0;
						// stage_hmac = stage_hmac_last;
					end
				end
			end
			endcase
		end
		
		endcase
	end
	
end

always @ (posedge clk) begin

	if (reset) begin
		H1_rst = 0;
	end else begin
	
		if (one_rst) begin
			one_rst = 0;
			H1_rst = 1;
		end
	
		case(H1_stage)
		
		0: begin 
			if (H1_push) begin
				if (data_byte != 8'h0a) begin	//LF
					H1_data_in = data_byte;
					H1_byte_rdy = 1;
				end else begin
					H1_byte_rdy = 0;
					H1_stage = 1;
				end
			end else begin
				H1_byte_rdy = 0;
			end
		end
		
		1: begin
			if (buff_cnt != 1) begin	//K <= B
				H1_stage = 2;
			end else begin				//K > B
				H1_byte_stop = 1;
				if (H1_done_hash) H1_stage = 2;
			end
		end
	
		2: begin
			if (H1_push) begin
				H1_rst = 0;
				H1_byte_stop = 0;
				H1_inp_idx = buff_cnt;
				pause_H1_rst = !pause_H1_rst;
				if (pause_H1_rst) begin
					H1_rst = 1;
					H1_byte_rdy = 1;
					H1_data_in = buff[H1_inp_idx -: 8] ^ 8'h36;
					H1_inp_idx = H1_inp_idx - 32'd8;
					H1_stage = 3;
				end
			end
		end
	
		3: begin	//Push Key + Ipad
			H1_data_in = buff[H1_inp_idx -: 8] ^ 8'h36;
			if (H1_inp_idx < 8) H1_stage = 4;
			else H1_inp_idx = H1_inp_idx - 32'd8;
		end
		
		4: begin	//Push Data Text
			if (H1_push) begin
				if (data_byte != 8'h0a) begin	//LF
					H1_data_in = data_byte;
					H1_byte_rdy = 1;
				end else begin
					H1_byte_rdy = 0;
					H1_byte_stop = 1;
					H1_stage = 5;
				end
			end else begin
				H1_byte_rdy = 0;
			end
		end
		
		5: begin	//Wait hashing done
			if (H1_done_hash && H1_push) H1_stage = 6; 
		end
	
		6: begin	//Reset
			if (H1_push) begin
				H1_rst = 0;
				H1_byte_stop = 0;
				H1_inp_idx = buff_cnt;
				pause_H1_rst = !pause_H1_rst;
				if (pause_H1_rst) begin
					H1_rst = 1;
					H1_byte_rdy = 1;
					H1_data_in = buff[H1_inp_idx -: 8] ^ 8'h5c;
					H1_inp_idx = H1_inp_idx - 32'd8;
					H1_stage = 7;
				end
			end
		end
	
		7: begin	//Push Key + Opad
			H1_data_in = buff[H1_inp_idx -: 8] ^ 8'h5c;
			if (H1_inp_idx < 8) H1_stage = 8;
			else H1_inp_idx = H1_inp_idx - 32'd8;
		end
	
		8: begin
			H1_byte_rdy = 0;
			H1_inp_idx = 32'd255;
			H1_stage = 9;
		end
		
		9: begin
			if(!H1_block_full) begin
				H1_byte_rdy = 1;
				H1_data_in = fst_hash[H1_inp_idx -: 8];
				if (H1_inp_idx < 8) begin H1_stage = 10;
				end else H1_inp_idx = H1_inp_idx - 32'd8;
			end
		end
		
		10: begin
			H1_byte_rdy = 0;
			H1_byte_stop = 1;
			if (H1_done_hash) begin
				H1_rst = 0;
				H1_stage = 0;
			end
		end
		
		endcase
	end
end

	// assign rx_done = State == 2;
	
	// always @ (posedge clk) begin
		// if (!Rst_n) begin
			// State = 0;
		// end else begin
			// case(State)
			
			// 0: begin
				// if (!Rx_Pin && RxEn) begin
					// start_bit = 1'b1;
					// counter = 4'b0000;
					// State = 1;
				// end
			// end
			
			// 1: begin
				// if (tick) begin
					// counter = counter+1;
	
					// if ((counter == 4'b1000) & (start_bit)) begin
						// start_bit = 1'b0;
						// counter = 4'b0000;
					// end

					// if ((counter == 4'b1111) & (!start_bit) & (Bit < NBits)) begin
						// Bit = Bit+1;
						// Read_data = {Rx_Pin,Read_data[7:1]};
						// counter = 4'b0000;
					// end
					
					// if ((counter == 4'b1111) & (Bit == NBits)  & (Rx_Pin)) begin
						// Bit = 4'b0000;
						// RxData[7:0] = Read_data[7:0];
						// counter = 4'b0000;
						// start_bit = 1'b1;
						// State = 2;
					// end
				// end
			// end
			
			// 2: begin
				// if (!Rx_Pin && RxEn) State = 0;
			// end
			
			// endcase
		// end
	// end


	// always @ (posedge clk)
	// begin
	// if (!Rst_n)	State = IDLE;
	// else 		State = Next;
	// end
	// always @ (posedge clk)
	// begin
		// case(State)	
		// IDLE: begin
			// if(!Rx_Pin & RxEn) begin
				// Next = READ;
				// RxDone = 1'b0;
			// end else Next = IDLE;
		// end
		// READ: begin
			// if(start_bit) begin
				// Next = IDLE;
				// RxDone = 1'b1;
			// end else Next = READ;
		// end
		// endcase
	// end
	// always @ (State) begin
		// case (State)
		// READ: read_enable = 1'b1;
		// IDLE: read_enable = 1'b0;			//If we get back to IDLE, we desable the read process so the "Tick always" could continue without geting Rx bits
		// endcase
	// end
	// always @ (posedge clk) begin
		// if (read_enable && tick) begin
			//RxDone = 1'b0;	
			// counter = counter+1;
	
			// if ((counter == 4'b1000) & (start_bit)) begin
				// start_bit = 1'b0;
				// counter = 4'b0000;
			// end

			// if ((counter == 4'b1111) & (!start_bit) & (Bit < NBits)) begin
				// Bit = Bit+1;
				// Read_data = {Rx_Pin,Read_data[7:1]};
				// counter = 4'b0000;
			// end
			
			// if ((counter == 4'b1111) & (Bit == NBits)  & (Rx_Pin)) begin
				// Bit = 4'b0000;
				//RxDone = 1'b1;
				// counter = 4'b0000;
				// start_bit = 1'b1;
			// end
		// end
	// end
	
	// always @ (posedge clk) begin
		// if (NBits == 4'b1000)
			// RxData[7:0] <= Read_data[7:0];	

		// if (NBits == 4'b0111) 
			// RxData[7:0] <= {1'b0,Read_data[7:1]}; 

		// if (NBits == 4'b0110) 
			// RxData[7:0] <= {1'b0,1'b0,Read_data[7:2]};	
	// end

endmodule