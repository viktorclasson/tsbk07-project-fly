for n = 1:1:10000000
    x(n)=2000*sin(n/100-0.5);
    y(n)=1000*sin(n);
end
plot(x,y,'b+');