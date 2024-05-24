`timescale 1ns / 1ps

module m_digest(clk,rst,counter_iteration,a_in,b_in,c_in,d_in,e_in,f_in,g_in,h_in,m_digest_final,done_digest
    );
	 inout reg [255:0] m_digest_final;
	 // output reg [255:0] m_digest_final;
	 output reg done_digest;
	 input clk,rst;
	 input [6:0] counter_iteration;
	 inout [31:0] a_in,b_in,c_in,d_in,e_in,f_in,g_in,h_in;
	 
	 reg temp_delay;
	 
	 reg [31:0] H0 ;
	 reg [31:0] H1 ;
	 reg [31:0] H2 ;
	 reg [31:0] H3 ;
	 reg [31:0] H4 ;
	 reg [31:0] H5 ;
	 reg [31:0] H6 ;
	 reg [31:0] H7 ;
	 
	 reg [31:0] temp_H0;
	 reg [31:0] temp_H1;
	 reg [31:0] temp_H2;
	 reg [31:0] temp_H3;
	 reg [31:0] temp_H4;
	 reg [31:0] temp_H5;
	 reg [31:0] temp_H6;
	 reg [31:0] temp_H7;
	 
	 
	always@(posedge clk) begin
		if(rst==0) begin
		 temp_delay=1'b0;
		 m_digest_final=256'd0;
		 done_digest= 1'b0;
		 
		 H0=32'h6a09e667;
		 H1=32'hbb67ae85;
		 H2=32'h3c6ef372;
		 H3=32'ha54ff53a;
		 H4=32'h510e527f;
		 H5=32'h9b05688c;
		 H6=32'h1f83d9ab;
		 H7=32'h5be0cd19;
		 
		 temp_H0=32'd0;
		 temp_H1=32'd0;
		 temp_H2=32'd0;
		 temp_H3=32'd0;
		 temp_H4=32'd0;
		 temp_H5=32'd0;
		 temp_H6=32'd0;
		 temp_H7=32'd0;
		end else begin
			if(counter_iteration==7'd64 && done_digest == 0) begin//32'bit modulo adder
				temp_H0=H0+a_in;
				if(temp_H0<32'hFFFFFFFF || temp_H0==32'hFFFFFFFF)H0=temp_H0;
				else H0=temp_H0-32'hFFFFFFFF;

				temp_H1=H1+b_in;
				if(temp_H1<32'hFFFFFFFF || temp_H1==32'hFFFFFFFF)H1=temp_H1;
				else H1=temp_H1-32'hFFFFFFFF;

				temp_H2=H2+c_in;
				if(temp_H2<32'hFFFFFFFF || temp_H2==32'hFFFFFFFF)H2=temp_H2;
				else H2=temp_H2-32'hFFFFFFFF;

				temp_H3=H3+d_in;
				if(temp_H3<32'hFFFFFFFF || temp_H3==32'hFFFFFFFF)H3=temp_H3;
				else H3=temp_H3-32'hFFFFFFFF;

				temp_H4=H4+e_in;
				if(temp_H4<32'hFFFFFFFF || temp_H4==32'hFFFFFFFF)H4=temp_H4;
				else H4=temp_H4-32'hFFFFFFFF;

				temp_H5=H5+f_in;
				if(temp_H5<32'hFFFFFFFF || temp_H5==32'hFFFFFFFF)H5=temp_H5;
				else H5=temp_H5-32'hFFFFFFFF;

				temp_H6=H6+g_in;
				if(temp_H6<32'hFFFFFFFF || temp_H6==32'hFFFFFFFF)H6=temp_H6;
				else H6=temp_H6-32'hFFFFFFFF;

				temp_H7=H7+h_in;
				if(temp_H7<32'hFFFFFFFF || temp_H7==32'hFFFFFFFF)H7=temp_H7;
				else H7=temp_H7-32'hFFFFFFFF;

				m_digest_final={H0,H1,H2,H3,H4,H5,H6,H7};
				done_digest = 1;
			end else if (counter_iteration!=7'd64) done_digest = 0;
		end
	end
endmodule
