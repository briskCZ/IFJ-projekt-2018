def whilefoo()
a = 1
b = "ahoj"
while a < 20 do
    print("prvni while\n")
    a = a + 2
    c = a
    print("c: ", c, "\n")
    while a < 5 do
        print("druhy while\n")
        d = c - 3
        a = d + 5
        print("d: ", d, "\n")
    end

end

end

whilefoo()
