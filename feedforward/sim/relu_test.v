`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/27/2020 01:03:16 PM
// Design Name: 
// Module Name: relu_test
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


module relu_test(

    );
    reg clk;
    reg [31:0] indata1;
    reg [31:0] indata2;
    reg deriv;
    reg rdy;
    wire [31:0] outdata;
    wire [31:0] outdata2;
    wire done;
    wire done2;
    
    
    initial begin
            clk = 0;
            rdy = 1;
            indata1 = 32'b00111111100000000000000000000000; // 1
            indata2 = 32'b10111111100000000000000000000000; // -1
            deriv = 0;
            forever #10 clk = ~clk;
        end
    
    // test positive value
    relu UUT1(
    .clk(clk),
    .rdy(rdy),
    .x_data(indata1),
    .z_data(outdata),
    .deriv(deriv),
    .done(done)
    );
    
    // test negative value
    relu UUT2(
    .clk(clk),
    .rdy(rdy),
    .x_data(indata2),
    .z_data(outdata2),
    .deriv(deriv),
    .done(done2)
    );
    
    
    
    // outdata1 should output indata1 -- outdata2 should output 0.
    
endmodule
