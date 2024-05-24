`timescale 1ns / 1ps

module m_scheduler(
	input clk,rst,flag_0_15,block_full,done_digest,
	input [31:0] data_in,
	output reg [31:0] mreg_15,
	output reg [6:0] iteration_out
);

		reg temp_case;
		reg [6:0] counter_iteration;
		(*S="TRUE"*) reg [31:0] mreg_0,mreg_1,mreg_2,mreg_3,mreg_4,mreg_5,mreg_6,mreg_7,mreg_8,mreg_9,mreg_10,mreg_11,mreg_12,mreg_13,mreg_14;
		wire [31:0] sigma_0,sigma_1,m_in_mreg15;
		
		assign sigma_0=({mreg_1[6:0],mreg_1[31:7]} ^ {mreg_1[17:0],mreg_1[31:18]} ^ mreg_1[31:0]>>2'b11);
		assign sigma_1=({mreg_14[16:0],mreg_14[31:17]} ^ {mreg_14[18:0],mreg_14[31:19]} ^ mreg_14[31:0]>>4'b1010);
		assign m_in_mreg15=(mreg_0 + sigma_0 + mreg_9 + sigma_1);
		always@(posedge clk) begin
		if(rst==0) begin
			temp_case<=1'b0;
			iteration_out<=7'd0;
			counter_iteration<=7'd1;
			mreg_15<=32'd0;
			mreg_14<=32'd0;
			mreg_13<=32'd0;
			mreg_12<=32'd0;
			mreg_11<=32'd0;
			mreg_10<=32'd0;
			mreg_9<=32'd0;
			mreg_8<=32'd0;
			mreg_7<=32'd0;
			mreg_6<=32'd0;
			mreg_5<=32'd0;
			mreg_4<=32'd0;
			mreg_3<=32'd0;
			mreg_2<=32'd0;
			mreg_1<=32'd0;
			mreg_0<=32'd0;
		end else begin
			//if(done_digest || (flag_0_15==0 && counter_iteration<17 && counter_iteration>0)) begin
			if (flag_0_15==0) begin
				mreg_15<=data_in;
				mreg_14<=mreg_15;
				mreg_13<=mreg_14;
				mreg_12<=mreg_13;
				mreg_11<=mreg_12;
				mreg_10<=mreg_11;
				mreg_9<=mreg_10;
				mreg_8<=mreg_9;
				mreg_7<=mreg_8;
				mreg_6<=mreg_7;
				mreg_5<=mreg_6;
				mreg_4<=mreg_5;
				mreg_3<=mreg_4;
				mreg_2<=mreg_3;
				mreg_1<=mreg_2;
				mreg_0<=mreg_1;
			end else begin
				if(counter_iteration!=64 && counter_iteration>0) begin
					mreg_15<=m_in_mreg15;
					mreg_14<=mreg_15;
					mreg_13<=mreg_14;
					mreg_12<=mreg_13;
					mreg_11<=mreg_12;
					mreg_10<=mreg_11;
					mreg_9<=mreg_10;
					mreg_8<=mreg_9;
					mreg_7<=mreg_8;
					mreg_6<=mreg_7;
					mreg_5<=mreg_6;
					mreg_4<=mreg_5;
					mreg_3<=mreg_4;
					mreg_2<=mreg_3;
					mreg_1<=mreg_2;
					mreg_0<=mreg_1;		
				end
			end
			
			if(counter_iteration==0 && block_full==0) begin
				counter_iteration<=7'd0;
			end else begin
				if(counter_iteration==64) begin
					counter_iteration=7'd64;
					iteration_out<=7'd64;
					if (done_digest && block_full==1 ) begin
						counter_iteration <= 7'd1;
						iteration_out <= 7'd0;
					end
				end else begin
					if(block_full==0) begin
						counter_iteration<=7'd1;
					end else begin
						case(temp_case)
						
						1'b0: begin
							temp_case = 1'b1;
						end
						1'b1: begin
							iteration_out = counter_iteration;
							counter_iteration = counter_iteration+7'd1;
						end
						
						endcase
					end
				end
			end
		end
		end

endmodule
