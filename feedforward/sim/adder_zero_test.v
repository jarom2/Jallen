`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/28/2020 01:12:05 AM
// Design Name: 
// Module Name: adder_zero_test
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


module adder_zero_test(

    );

    reg clk, rdy1, rdy2;
    reg [31:0] indata1;
    reg [31:0] indata2;
    
    reg [31:0] indata3;
    reg [31:0] indata4;
    
    wire [31:0] outdata;
    wire [31:0] outdata2;
    wire done;

    initial begin
        clk = 0;
        rdy1 = 1;
        rdy2 = 1;
        indata1 = 32'b01000000000000110011001100110011; // 2.04999
        indata2 = 32'b00000000000000000000000000000000; // 0;
        
        indata3 = 32'b00000000000000000000000000000000; // 0;
        indata4 = 32'b00111111100000101000111101011100; // 1.01999
        forever #10 clk = ~clk;
    end

    adder UUT1(
        .clk(clk),
        .done(done),
        .x_rdy(rdy1),
        .y_rdy(rdy2),
        .x_data(indata1),
        .y_data(indata2),
        .z_data(outdata)
    );
    
    adder UUT2(
        .clk(clk),
        .done(done),
        .x_rdy(rdy1),
        .y_rdy(rdy2),
        .x_data(indata3),
        .y_data(indata4),
        .z_data(outdata2)
        );

    // outdata should contain 01000000010001000111101010001101 (40447a8d) (3.06998)

endmodule
