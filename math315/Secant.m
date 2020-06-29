function x = Secant(f,x0,x1,tol,nmax)
%
% Input: f - an anonymous function or function handle for f(x)
%    x0,x1 - two initial guesses of the root,
%      tol - a (relative) stopping tolerance, and
%     nmax - specifies maximum number of iterations.
%
% Output: x - a vector containing the iterates, x0, x1, ...    

x(1)=x0;
x(2)=x1;

for n=2:nmax
    m=(f(x(n))-f(x(n-1)))/(x(n)-x(n-1));
    x(n+1)=x(n)-f(x(n))/m;
    if (abs(x(n+1)-x(n))<=tol*abs(x(n)))
        break
    end
end

x=x(:);

