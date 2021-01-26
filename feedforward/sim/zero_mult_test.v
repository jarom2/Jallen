`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/25/2020 09:55:00 AM
// Design Name: 
// Module Name: mult_test
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module zero_mult_test(

    );

    reg clk;
    reg rdy1, rdy2;
    reg [31:0] indata1;
    reg [31:0] indata2;
    reg [31:0] indata3;
    reg [31:0] indata4;
    wire [31:0] outdata1;
    wire [31:0] outdata2;
    wire done1;
    wire done2;

    initial begin
        clk = 0;
        rdy1 = 1;
        rdy2 = 1;
        indata1 = 32'b00000000000000000000000000000000; // 0
        indata2 = 32'b00111111100000101000111101011100; // 1.01999
        
        indata3 = 32'b01000000000000110011001100110011; // 2.04999
        indata4 = 32'b00000000000000000000000000000000; // 0
        forever #10 clk = ~clk;
    end

    multiplier UUT1(
        .clk(clk),
        .done(done1),
        .x_rdy(rdy1),
        .y_rdy(rdy2),
        .x_data(indata1),
        .y_data(indata2),
        .z_data(outdata1)
    );

    multiplier UUT2(
        .clk(clk),
        .done(done2),
        .x_rdy(rdy1),
        .y_rdy(rdy2),
        .x_data(indata3),
        .y_data(indata4),
        .z_data(outdata2)
    );
    
    // outdata1 and outdata2 should contain (0)

endmodule
