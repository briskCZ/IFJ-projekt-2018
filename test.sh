echo "_____________TESTS_____________"

for FILE in ./tests/*;
do
    if [ $FILE != "./tests/ifj18.rb" ]; then
        echo "--------------------------------------------------------"
        echo "using main: " "$FILE"
        ./main < "$FILE" 1> out.asm ; ./ic18int ./out.asm > result_our
        echo $? "< our retval"
        echo "using ruby: " "$FILE"
        ruby -r ./tests/ifj18.rb "$FILE" > result_ruby
        diff ./result_our ./result_ruby > /dev/null
        DIFF=$?
        if [ $DIFF -eq 1 ]; then
            echo "different output from file: " $FILE
        fi
    fi
done
