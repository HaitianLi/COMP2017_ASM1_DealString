#!/bin/bash
###
 # @Author: Haitian Li
 # @Date: 2021-03-16 18:56:21
 # @LastEditors: Haitian Li
 # @LastEditTime: 2021-03-17 20:04:10
 # @Description: I do not wish to be horny anymore
 #               I just want to be happy.
### 

##The scrip automatically check all the cases in "tests" folder

#################
#Cases for Invalid Command line argument
#################

    # arguments more than 4
    # arguments less than 4
    # arguments have invalid time format


#################
#Cases for Entries
#################
    #Invalid format
        #::: 
        #2. : 
        #3. no : 
        #4. :::::
        #5.  ::::
    #Invalid time format
        #::
        #2. : missing
        #3. ::: more 
        #4. missing mnumber
        #5. right format but wrong value 

    #Large_Entry
        #Invalid format with more than 4096(check the garbage) clean garbage tick
        #2. Invalid 4096 with valid garbage(check the garbage been cleaned, the entry be marked as invalid) tick


    #Valid entry and valid time
        #1 station
            #1.normal
            #2.normal
            #3.not sorting order
            #4.earlyest time for the day
            #5.unsorted, no destination
            #6.right on time
        #2 station
            #1.normal
            #2.normal
            #3.not sorting order
            #4.earlyest time for the day
            #5.unsorted, no destination
            #6 right on time

    #No timetable to process(no data)


echo "##########################"
echo "### Testing"
echo "##########################"
count=0 


for test in tests/*.in; do
name=$(basename $test .in)
expected=tests/$name.out
realOutput=tests/$name.real
args=tests/$name.args

cat $test | xargs -a $args ./timetable &> $realOutput
diff $realOutput $expected
rm $realOutput

if [ $? -eq "1" ]; then
    echo -e "\033[;31m [Failed] $name\033[0m"
else
	echo -e "\033[;32m [Passed] $name\033[0m"
fi

count=$((count+1))
done

echo "Finished running $count tests!"




