function [xvals,yvals,A] = SurfaceArea(x,y,m)
% Input: x the x-values to fit the parametric spline
%        y the y-values to fit the parametric spline
%        m the parameter defining the length
%
%
%Output: xvals the xvalues from the parametric spline 
%        yvals the yvalues from the parametric spline
    
    n = length(x);
    t = (n);
    t(1) = 0;
    for i=2:n
        t(i) = t(i-1)+sqrt((x(i)-x(i-1))^2 + (y(i)-y(i-1))^2);
    end

    tvals = linspace(t(1),t(n), m);
    
    % Define spline for x coordinate
    sx = spline(t,x);
    xvals = ppval(sx,tvals);
    
    % Define spline for y coordinate
    sy = spline(t,y);
    yvals = ppval(sy,tvals);
    
    coefsx = sx.coefs;
    coefsy = sy.coefs;
    A = 0;
    for i = 1:n-1
        f_A = @(z)(coefsx(i,4) + coefsx(i,3).*(z-t(i)) + coefsx(i,2).*((z-t(i)).^2) + coefsx(i,1).*((z-t(i)).^3)) .*((coefsy(i,3)+2.*coefsy(i,2).*(z-t(i))+3.*coefsy(i,1).*((z-t(i)).^2)).^2 + ((coefsx(i,3)+2.*coefsx(i,2).*(z-t(i)) + 3.*coefsx(i,1).*((z-t(i)).^2)).^2)).^0.5;
        A = A + quad(f_A,t(i),t(i+1));
    end

    A = 2*pi*A;

    
end
