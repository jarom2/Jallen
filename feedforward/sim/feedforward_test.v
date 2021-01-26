`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/28/2020 09:38:47 AM
// Design Name: 
// Module Name: feedforward_test
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


module feedforward_test(

    );
    
    reg clk;
    reg [31:0] x1, x2, x3, x4;
    reg [31:0] w1, w2, w3, w4;
    wire [31:0] z1;
    wire done;
    
    initial begin
        clk = 0;
        x1 = 32'b01000000000000110011001100110011; // 2.04999
        x2 = 32'b01000000000000110011001100110011; // 2.04999
        x3 = 32'b01000000000000110011001100110011; // 2.04999
        x4 = 32'b01000000000000110011001100110011; // 2.04999
        
        w1 = 32'b00111111100000101000111101011100; // 1.01999
        w2 = 32'b00111111100000101000111101011100; // 1.01999
        w3 = 32'b00111111100000101000111101011100; // 1.01999
        w4 = 32'b00111111100000101000111101011100; // 1.01999
        
        forever #10 clk = ~clk;
    end
    
    feedforward UUT(
        .clk(clk),
        .result(z1),
        .done(done),
        .x1(x1),
        .x2(x2),
        .x3(x3),
        .x4(x4),
        .w1(w1),
        .w2(w2),
        .w3(w3),
        .w4(w4)
    );
    
endmodule
