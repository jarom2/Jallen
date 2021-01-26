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


module mult_test(

    );

    reg clk, rdy1, rdy2;
    reg [31:0] indata1;
    reg [31:0] indata2;
    wire [31:0] outdata;
    wire done;

    initial begin
        clk = 0;
        rdy1 = 1;
        rdy2 = 1;
        indata1 = 32'b01000000000000110011001100110011; // 2.04999
        indata2 = 32'b00111111100000101000111101011100; // 1.01999
        forever #10 clk = ~clk;
    end

    multiplier UUT(
        .clk(clk),
        .done(done),
        .x_rdy(rdy1),
        .y_rdy(rdy2),
        .x_data(indata1),
        .y_data(indata2),
        .z_data(outdata)
    );

    // outdata should contain 01000000000001011101001011110001 (2.0909)

endmodule
