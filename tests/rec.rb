def foo(a)
	if a < 5 then
		print a, "\n"
		a = a+1
		foo(a)
	else
	end
	a = 3
end

b = 5-5
x = foo(b)

print "-----------\n"
print x, "\n"
