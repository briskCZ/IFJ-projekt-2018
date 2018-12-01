def whilefoo()
a = 1
b = "ahoj"
while a < 20 do
    print("prvni while\n")
    c = a + 2
    print("c: ", c, "\n")
    while a < 5 do
        print("druhy while\n")
        d = c - 3
        print("d: ", d, "\n")
    end

end

end

whilefoo()
