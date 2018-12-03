def fib(n)
    i = 0
    j = 1
    k = 1
    while (n >= 3) do
        print "inwhile\n"
        n = n - 3
        i = j + k
        j = i + k
        k = i + j
    end
    if (n == 0) then
        print("vracam i\n")
        i
    else
        if (n == 1) then
            print "vracam j\n"
            j
        else
            print "vracam k\n"
            k
        end
    end
end


n = 10
rv = fib n
print rv, "\n"
