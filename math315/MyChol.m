function L = MyChol(A)

n = size(A, 1);
L = zeros(n, n);
for k = 1:n
    L(k,k) = sqrt(A(k,k));
    L(k+1:n, k) = A(k+1:n, k)/L(k,k);
    A(k+1:n, k+1:n) = A(k+1:n, k+1:n) - L(k+1:n, k)*L(k+1:n, k).';
end


% scott weitzner 516-306-6048
% Chao Zhang 404-944-4909