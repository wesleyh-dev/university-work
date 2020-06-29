function [xvals,yvals,L] = ParSpline(x,y,m)
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

    tvals = linspace(t(1),t(n),m);
    
    % Define spline for x coordinate
    sx = spline(t,x);
    xvals = ppval(sx,tvals);
    
    % Define spline for y coordinate
    sy = spline(t,y);
    yvals = ppval(sy,tvals);
    
    coefsx = sx.coefs;
    coefsy = sy.coefs;
    L = 0;
    for i = 1:n-1
        f_v = @(z)((coefsx(i,4) + coefsx(i,3).*(z-t(i)) + coefsx(i,2).*((z-t(i)).^2) + coefsx(i,1).*((z-t(i)).^3)).^2).*(coefsy(i,3) + coefsy(i,2).*(2*(z-t(i))) + coefsy(i,1).*(3*(z-t(i))).^2);
        L = L + quad(f_v,t(i),t(i+1));
    end

    L = pi*L;

    
end

