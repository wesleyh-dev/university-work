x=[0,0.788837068786060,1.26213931005770,1.57767413757212,3.62865051641587,2.20874379260097,3.15534827514424,0.788837068786060,1.18325560317909,0.591627801589545];
y=[0,0,0.236651120635818,1.18325560317909,2.49090314653968,3.80452204729864,5.32728259155826,7.01397782566082,8.81392837524268,10.6731238272497];

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
colormap('default')

