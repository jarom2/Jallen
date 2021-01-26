`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/28/2020 08:55:42 AM
// Design Name: 
// Module Name: feedforward
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


module feedforward(
    input [31:0] x1, x2, x3, x4,
    input [31:0] w1, w2, w3, w4,
    input clk,
    output wire [31:0] result,
    output wire done
    );
    
    wire [32:0] T1, T2, T3, T4, T5, T6, T7;
    
    reg rdy1;
    reg rdy2;
    reg deriv;
    
    initial begin
        rdy1 = 1;
        rdy2 = 1;
        deriv = 0;
    end
    
    multiplier m1(
        .clk(clk),
        .x_rdy(rdy1),
        .y_rdy(rdy2),
        .x_data(x1),
        .y_data(w1),
        .z_data(T1[32:1]),
        .done(T1[0])
    );
    
    multiplier m2(
        .clk(clk),
        .x_rdy(rdy1),
        .y_rdy(rdy2),
        .x_data(x2),
        .y_data(w2),
        .z_data(T2[32:1]),
        .done(T2[0])
    );
    
    multiplier m3(
        .clk(clk),
        .x_rdy(rdy1),
        .y_rdy(rdy2),
        .x_data(x3),
        .y_data(w3),
        .z_data(T3[32:1]),
        .done(T3[0])
    );
    
    multiplier m4(
        .clk(clk),
        .x_rdy(rdy1),
        .y_rdy(rdy2),
        .x_data(x4),
        .y_data(w4),
        .z_data(T4[32:1]),
        .done(T4[0])
    );  
    
    adder a1(
        .clk(clk),
        .x_rdy(T1[0]),
        .y_rdy(T2[0]),
        .x_data(T1[32:1]),
        .y_data(T2[32:1]),
        .z_data(T5[32:1]),
        .done(T5[0])
    );
    
    adder a2(
        .clk(clk),
        .x_rdy(T3[0]),
        .y_rdy(T4[0]),
        .x_data(T3[32:1]),
        .y_data(T4[32:1]),
        .z_data(T6[32:1]),
        .done(T6[0])
    );
    
    adder a3(
        .clk(clk),
        .x_rdy(T5[0]),
        .y_rdy(T6[0]),
        .x_data(T5[32:1]),
        .y_data(T5[32:1]),
        .z_data(T7[32:1]),
        .done(T7[0])
    );
        
    relu r1(
        .clk(clk),
        .rdy(T7[0]),
        .x_data(T7[32:1]),
        .z_data(result),
        .deriv(deriv),
        .done(done)
    );
    
endmodule
