def iftest(x)
    if x >= 5 then
        print "vetsi nez 5\n"
        if (x >= 10) then
            print "vetsi nez 10\n"
            15
        else
            print "vetsi nez 5 menzi nez 10\n"
            7.5
        end
    else
        print "mensi nez 5\n"
        0
    end
end
def ififtest(x)
    if x >= 5 then
        if (x >= 10) then
            if (x <= 20) then
                "10-20"
            else
                if (x <= 30) then
                    "20-30"
                else
                    ">30"
                end
            end
        else
            "5-10"
        end
    else
        "<5"
    end

end
def ifwhiletest(x)
    a = x
    while a < x + 5 do
        a = a + 1
        print "a in while ", a, "\n"

    end

end
x = 4
ret = iftest x
print "retval iftest",ret, "\n"

x = 5
ret = ififtest x
print "retval ififif test: ", ret, "\n"

x = 6
ret = ifwhiletest x
print "retval ifwhiletest:  ", ret, "\n"
