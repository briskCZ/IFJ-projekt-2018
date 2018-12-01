def fact(n)
	if n < 2 then
		result = 1
	else
		new_n = n - 1
		temp = fact new_n
		res = n * temp
	end
	result
end

x = fact 5
print x, "\n"
