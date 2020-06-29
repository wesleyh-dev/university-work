function x = Bisection(f, a, b, tol, nmax)
  
% Input: f - an anonymous function or function handle for f(x)
%     [a,b]- an initial bracket containing the root
%      tol - a (relative) stopping tolerance, and
%     nmax - specifies maximum number of iterations.
%
% Output: x - a vector containing the iterates, x0, x1, ...    

x(1)=a;
x(2)=b;

for n=2:nmax
    m=(a+b)/2;
    x(n+1)=m;
    if f(a)*f(m)<0
        b=m;
    else
        a=m;
    end
    
    if (abs(b-a)<=tol*abs(a))
        break
    end
end
x=x(:);
    
end
