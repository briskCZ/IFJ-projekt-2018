def sum_first_num(n)
	i = 0
	sum = 0
	while i <= n do
		sum = sum + i
		i = i + 1
	end
	sum
end

result = sum_first_num 10

print result, "\n"	

