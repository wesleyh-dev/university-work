function x = Newton(f,df,x0,tol,nmax)
%
% Input: f - an anonymous function or function handle for f(x)
% df - an anonymous function of function handle for f’(x)
% x0 - an initial guess of the root,
% tol - a (relative) stopping tolerance, and
% nmax - specifies maximum number of iterations.
%
% Output: x - a vector containing the iterates, x0, x1, ...
x(1)=x0;

for n=1:nmax
    x(n+1)=x(n)-f(x(n))/df(x(n));
    if (abs(x(n+1)-x(n))<=tol*abs(x(n)))
        break
    end
end

x=x(:);

end
