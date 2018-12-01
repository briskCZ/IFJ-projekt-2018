str = "abcdefgh"

len = length(str)
i = 0
while i < len do
	x = substr(str, 0, i)
	print x, "\n"
	i = i + 1
end
