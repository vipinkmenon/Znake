`define clockFreq 100000000 //In Hertz
`define minPulseWidth 0.01
//`define clockCount `minPulseWidth*`clockFreq

module debounce(
input       i_clk,
input       i_button,
output reg  o_press
);

integer counter=0;
parameter clockCount = $floor(`minPulseWidth*`clockFreq);

always @(posedge i_clk)
begin
    if((counter < clockCount)&i_button)
        counter <= counter+1;
    else if(!i_button)
        counter <= 0;
end

always @(posedge i_clk)
begin
    if(counter == clockCount-1)
        o_press <= 1'b1;
    else
        o_press <= 1'b0;
end


endmodule
