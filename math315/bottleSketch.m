% x = [0 1.5 2.5 2 2 3 3.3 3 2 2.5 2 1 1 1.3 1 1.05 1 0];
% y = [0 0 0.5 3 4 5 6.5 8 9 10 11 12 13 13.4 14.5 14.75 15 15];
x = [0    1.4049    2.3415    1.8732    1.8732    2.8099    3.0908    2.8099    1.8732    2.3415    1.8732    0.9366    0.9366  1.2176    0.9366    0.9834    0.9366    0];
y = [0  0   0.4683  2.8099  3.7465  4.6831  6.0880  7.4929  8.4296  9.3662  10.3028 11.2394 12.1760 12.5507 13.5810 13.8151 14.0493 14.0493];

t = 1:length(x);
tvals = linspace(t(1), t(end), 500);
xvals = spline(t, x, tvals);
yvals = spline(t, y, tvals);
set(gcf, 'color', 'white')
for i = 1:length(tvals)/5
    plot(xvals(1:5*i), yvals(1:5*i), 'LineWidth', 2) 
    drawnow
end
axis equal
axis off

theta = 0:pi/20:2*pi;
r= xvals;
z= yvals;
xx= bsxfun(@times, r', cos(theta));
yy= bsxfun(@times, r', sin(theta));
zz= repmat(z', 1, length(theta));
figure, surf(xx,yy,zz)
axis equal
colormap(winter)
