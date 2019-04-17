`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/13/2019 02:34:17 PM
// Design Name: 
// Module Name: my_uart_rx
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


module my_uart_rx(
    input rx,
    input clk,
    input rst_l,
    output reg rdy,
    output reg [7:0] data
    );
    
    reg SCLK;
        
    reg [3:0] bit;
    reg [1:0] state;      
    reg [31:0] sclk_count;
        
    initial begin
        state = 0;
        sclk_count = 0;
        SCLK = 0;
        bit = 0;
    end
        
    always @(posedge clk) begin 
        if (sclk_count > 5208) begin //100MHz input
            SCLK       <= ~SCLK; //we need 9600Hz
            sclk_count <= 0;
        end
        else begin
            sclk_count <= sclk_count + 1;
        end
    end
    
    //RX state machine
    always @(posedge SCLK) begin
    if (~rst_l) begin
        state <= 0;
    end
    else begin
        case(state)
        0://wait
            begin
                if(rx) begin
                    rdy <= 0;
                    state <= 0;
                end
                else begin
                    state <= 1;
                    bit <= 0;
                end
            end
        1: //receive
            begin
                if(bit < 7) begin
                    data[bit] <= rx;
                    state <= 1;
                    bit <= bit + 1;
                end
                else begin
                    data[bit] <= rx;
                    rdy <= 1;
                    state <= 0;
                end
            end
        endcase
    end
    end
endmodule
