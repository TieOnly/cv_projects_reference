`timescale 1ns / 1ps

module m_pader_parser(
	input clk,rst,byte_rdy,byte_stop,done_digest,
	input [7:0] data_in, 
	input [6:0] counter_iteration,
	output reg [63:0] addr_cnt,
	output reg flag_0_15,
	output reg [31:0] padd_out,
	output reg block_full, done_hash
);

	reg [7:0] block_512 [63:0];
	reg [7:0] addr_block;
	reg [63:0] m_size;
	reg temp_chk;
	reg padd_done;
	
	reg [31:0] add_out0;
	reg [31:0] add_out1;
	reg [31:0] add_out2;
	reg [31:0] add_out3;
	
	initial begin
		addr_cnt = 64'd0;
	end
	
	always@(posedge clk) begin
	if(rst==0) begin
		add_out0=7'd0;
		add_out1=7'd1;
		add_out2=7'd2;
		add_out3=7'd3;
		addr_block=8'd0;
		addr_cnt=64'd0;
		block_full = 1'b0;
		m_size=64'd0;
		padd_out=32'd0;
		padd_done = 1'b0;
		temp_chk=1'b0;
		flag_0_15=1'b0;
		done_hash = 0;
	end else begin
	
		if(byte_rdy) begin

			if (addr_block < 64) begin
				if (addr_block == 63) block_full = 1'b1;
				block_512[addr_block] = data_in;
				addr_block = addr_block+1;
				addr_cnt = addr_cnt+1;
			end
			
		end else begin
			
			if(byte_stop) begin
				if(temp_chk==0) begin
					m_size[63:0]=(addr_cnt)*8;
					
					if (addr_block <= 64 - 8) begin
						block_512[addr_block]=8'b1000_0000;
						addr_block=addr_block+1;
						padd_done = 1'b1;
					end else begin
						padd_done = 1'b0;
						if (addr_block < 64) begin
							block_512[addr_block]=8'b1000_0000;
							addr_block=addr_block+1;
						end
					end
					temp_chk=1'b1;
				
				end else begin
				
					if (padd_done) begin
						if (addr_block <= 64 - 8) begin
							block_512[addr_block]=8'd0;
							addr_block=addr_block+1;
						end else begin
							block_full = 1'b1;
							block_512[63]=m_size[7:0];
							block_512[62]=m_size[15:8];
							block_512[61]=m_size[23:16];
							block_512[60]=m_size[31:24];
							block_512[59]=m_size[39:32];
							block_512[58]=m_size[47:40];
							block_512[57]=m_size[55:48];
							block_512[56]=m_size[63:56];
						end
						
					end else begin
						if ( addr_block < 64 ) begin
							if (addr_block == 63) block_full = 1'b1;
							block_512[addr_block]=8'd0;
							addr_block=addr_block+1;
						end
					end
				
				end
				
			end else begin
				//avoiding stop byte check to keep less out flags
			end
			
			
		end
		 
		if(block_full) begin
			if(add_out3<64) begin
			
				padd_out[7:0]=block_512[add_out3];
				padd_out[15:8]=block_512[add_out2];
				padd_out[23:16]=block_512[add_out1];
				padd_out[31:24]=block_512[add_out0];
				
				add_out0=add_out0+7'd4;
				add_out1=add_out1+7'd4;
				add_out2=add_out2+7'd4;
				add_out3=add_out3+7'd4;
			
			end else if (counter_iteration == 64) begin
				flag_0_15=1'b0;
				
				if (padd_done) done_hash = 1'b1;
				else begin
					add_out0=7'd0;
					add_out1=7'd1;
					add_out2=7'd2;
					add_out3=7'd3;
					addr_block=8'd0;
					block_full = 1'b0;
					
					if (byte_stop) begin
						padd_done = 1'b1;
						if (0 == addr_cnt % 64) begin
							block_512[0] = 8'b1000_0000;
							addr_block = 8'd1;
						end
					end
				end
				
			end else if (flag_0_15 == 0) begin
				flag_0_15=1'b1;
			end
		end else begin
		
		end
	end
	end
endmodule
