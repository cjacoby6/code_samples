Cori Jacoby
HW6
README

11/16/14

This project in my implementation of a 14-instruction universal machine that is 
Turing complete. It was created in roughly a week to satisfy a homework project
for the Computer Science 40 course at Tufts University, Machine Structure and 
Assembly Language Programming. This rest of this documents in more detail the 
exact architecture of the project as well as testing. I have also included 
other tests (sandmark, codex, and advent), supplied with the assignment, to fully
demonstrate the functionality of the project. 

Acknowledgements: Course Staff
		  Naomi Zarilli and Alex Ravan (debugging)

Architecture: My program has a relatively simply architecture. The outer shell
		of the program is the um module. This c file initializes, frees
		and runs the program from the main function. For running, it
		extracts the opcode and relevant values which it passes to the
		instruction module. The instruction module handles each
		each instruction but does not deal with memory and sequences. 
		For the instruction that did accesss the memory, the instruction
		module calls the segments module which deals with all memory 
		access, including the segment value struct. 
		
		All in all the only part of the program that deals with the
		actual sequence is the segments module. The exception is
		the initialize um function as it was far less efficient for it
		call those functions to read in the initial file.

Performance Analysis: I estimate my UM to be able to perform roughly 50 million
		      instruction in 1.7 seconds.
		      I based this analysis off of the user time it took to run
		      midmark.um. As midmark is roughly 80 million instructions
		      and took 2.27 seconds, I figured my UM could run 50
		      million instructions in roughly 62.5% of that time.

UMTESTS:

	add.um
		add.um tests the add function and relies on the previously
		tested instructions load value, output, and halt. It does this
		by loading two ASCII values and adding them. It then prints the
		equation and result followed by a newline. If it prints the
		result as anything other than the previously computed value, the
		add instruction has failed.
	cmov.um
		cmov.um test the conditional move instruction and relies on the
		previously tested load value and output instructions. It works
		by continually inputting two characters and putting them in the
		conditional move operation based on the value in another
		register. If the test passes it will print "Yes!\n". The test
		tests both cases of the conditional move, when it moves the
		value and when it doesn't and this two cases are represented in
                the output. If the conditional move on a 0 fails it prints Bad
		instead of Yes and if it fails on a non-zero value the
		punctuation differs.
	divide.um
		divide.um tests the divide function and uses the previously
		tested instructions load value, output, and halt. It does this
		by loading in two values and dividing them. It then works
		similarly to the multiply test by printing '=' followed by the
		result, which in this case should be '4' and then the character
		'\n'. If the output is anything else the divide instruction has
		failed.
	goto.um
		goto.um tests the load program instruction when the program
		stays the same and only the counter changes. It uses additional
		instructions load value, output, and halt. It works by loading
                in the value of the line number of a later instruction and then
		running load program with that and the segment with the new
		program remaining at 0. If the test outputs "GOTO failed.\n" the
		instruction has failed and if it outputs "GOTO passed.\n" the 
		load program instruction, as well as the other instructions,
		have passed and are at least somewhat functional.
	halt.um
		halt.um tests the halt instruction and is a provided test. It
		works by first running the halt instruction. If the test outputs
		anything it means that the instruction has failed as it has 
		continued on to the load value and output instructions. If it
		passes it won't output anything.
	input.um
		input.um tests the input instruction using the previously tested
		output, load value, and halt instructions. It works by inputting
		a value from stdin and printing it followed by a newline. If the
		test prints any other value then the input instruction has
		failed.
	loadprog.um
		loadprog.um tests the load program instruction. Specifically it
		ensures that the values are copied over fully and if the
		previous segment that was the new program is unmapped the
                instruction still functions. It relies on the previously tested
                instructions load values, map segment, segment load, segment
                store, output, and halt. It works by loading in several values
                and mapping a new segment of size three. It then proceeds to
                load the instructions from later in the program (unmap segment,
                output register 1, and halt) into the new segment, in the
                previously specified order. It then loads the new segment as the
                new program. Previously 'Y' was stored in the to-be-outputted
                register. If load program does not work the program will
                continue to run the original instructions which load 'F' into
                the to-be-outputted register. If the test outputs "Y" the
                instruction has passed and if it prints "F" or does anything
                else it has failed.
	load.um
		load.um tests the load value instruction and relies on the
		previously tested instruction halt. It works by loading in two
		values and then runs the halt command. If it does anything other
		than printing nothing the load value instruction has failed.
	map.um
		map.um tests the map segment instruction and relies on the 
		previously tested load value, segment store, segment load and 
		halt instructions. It does this by loading the values of the 
		string "Passed!\n" then making a segment of that size. It then
		stores those values in the new segment in order. Finally, it 
		loads those values back out of memory and outputs them. If the
		test does anything other than output "Passed!\n" the map segment
		instruction has failed.
	multiply.um
		multiply.um tests the multiply instruction and uses the
		previously tested instructions load value, output, and halt. It
		prints '=' followed by the result of the multiplication and then
		a new line. If the result is anything other than the previously
		computed value of the ASCII multiplication as the value, the 
		multiply instruction has failed. With the specified input file
		it should print "A\n".
	nand.um
		nand.um tests the bitwise nand instruction and uses the
		previously tested load value and output instructions.
		It does this by loading in two values and nanding them.
		Because the nand of any numbers that are less than 255 results
		in a number greater than 255 and therefore not output-able
		in the UM format and I wanted to input two ASCII values, I
		nanded the two values and then nanded the result again. This
		essentially not-ed the result of the nand which returned it to
		the bitwise and of the two values. This value was then outputted
		followed by a new line. In this case, the passing output is
		"d\n" and anything else constitutes a failure.
	output.um
		output.um tests the loadput instruction and uses the previously
		tested instructions load value and halt. It loads in and outputs
		values. If it passes it will output "Out\n"and if it outputs
		anything else the function has failed. 
	seg_load.um
		seg_load.um tests the segment load instruction and uses the
		previously tested instructions load value, conditional move,
		and halt. It loads the value in memory 0,19 (which as cmov
		with register 0 as all the values should be 0) and then performs
		a series of conditional moves and with the register that loaded 
		the value and outputs. As long as the instruction loaded the 0 
		correctly the output is "dddd\n", else it prints "Fail\n".
	seg_store.um
		seg_store.um tests the segment store instruction and uses the
		previously tested instructions load value, segment load, output
		multiply, and halt. The tests works by loading in two ASCII 
		values and then storing them in memory. Following that it
		outputs the values. Then it loads two numbers and multiply them.
                Together they are the value of the halt instruction. It then
                stores that value in the next instruction line. If the program
                prints the two previously loaded values and then stops the
                instruction passes the test. If it fails it will with either
                segfault or continue on and print out "NOOO!!" before halting.
	unmap.um
		unmap.um tests the unmap segment instruction using the
		previously tested instructions load value, map segment, segment
		store, segment load, and output instructions. It works by
                mapping two new segments and then loading in values and storing
                them in the first newly mapped segment. It then unmaps that
                segment and then immediately makes a new segment with a
                different size. After loading in and storing a new value in that
                segment, it outputs the value. If the test outputs anything
                other than "Y\n" the unmap instruction has failed and is not
                working as it should.
