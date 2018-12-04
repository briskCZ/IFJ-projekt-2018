def foo()
    i = 0
    x = 15
    while i < x do
        print i, "\n"
        i=i+1
    end
end

#foo

def boo(str, max)
    print("aha", str)
    len = length(str)
    #tmp = "ahoj"
    space = "\t"
    while len < max do
        ret = ret + str + space
        len = length(ret)
    end
    ret
end

def hoo(a, a)
end
max = 20
string = "ah"
boo string, max
