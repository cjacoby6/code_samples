from time import sleep

class GameOfLife:
	board = []
	height = 0
	width = 0 
	tick = 1
	iterations = 40

	def __init__(self, startboard, tick, iters):
		self.board = startboard
		if tick:
			try:
				self.tick = int(tick)
			except:
				pass
		if iters:
			try:
				self.iterations = int(iters)
			except:
				pass

	def validateBoard(self):
		#check that something was entered
		if self.board == []:
			print "Nothing was entered so Conway's Game of Life cannot start"
			return False

		#check that all values are 0 or 1
		for row in self.board:
			for value in row:
				if value != '0' and value != '1':
					if value == '':
						# false positive - remove this value caused by extra whitespace
						row.remove(value)
					print "There is a value in the board that is not 0 or 1 so Conway's Game of Life cannot start"
					return False

		#check that all rows have the same length
		numrows = 0
		firstlen = len(self.board[0])
		for row in self.board:
			if len(row) != firstlen:
				print "All rows do not match in length so Conway's Game of Life cannot start"
				return False
			numrows += 1

		#set height and width
		self.height = numrows
		self.width = firstlen
		
		# no issues found so return true
		return True

	def printBoard(self):
		print '+-' * self.width + '+'
		for row in self.board:
			printRow = []
			for value in row:
				if value == '0':
					printRow.append(' ')
				else:
					printRow.append('X')
			print '|' + '|'.join(printRow) + '|'
			print '+-' * self.width + '+'
			# print ' '.join(row)


	def countNeighbors(self, i, j):
		count = 0

		up = (i - 1) % self.height
		curRow = i
		down = (i + 1) % self.height

		left = (j - 1) % self.width
		curCol = j
		right = (j + 1) % self.width

		count += int(self.board[up][left])
		count += int(self.board[up][curCol])
		count += int(self.board[up][right])
		count += int(self.board[curRow][left])
		count += int(self.board[curRow][right])
		count += int(self.board[down][left])
		count += int(self.board[down][curCol])
		count += int(self.board[down][right])

		return count

	def newValue(self, i, j):
		neighbors = self.countNeighbors(i, j)
		if self.board[i][j] == '1':
			if neighbors == 2 or neighbors == 3:
				return '1'
		else:
			# if not '1' then '0' and dead so only is alive if exactly 3 alive neighbors
			if neighbors == 3:
				return '1'
		# if not set to alive return dead
		return '0'

	def newBoard(self):
		newBoard = []
		for i, row in enumerate(self.board):
			newRow = []
			for j, cell in enumerate(row):
				newRow.append(self.newValue(i, j))
			newBoard.append(newRow)
		return newBoard

	def start(self):
		count = 0
		while count < self.iterations:
			print count
			self.printBoard()
			self.board = self.newBoard()
			sleep(self.tick)
			count += 1

def main():
	board = []
	tick = raw_input("Please enter the print tick time, if nothing is entered or entered value is not an integer, default is 1 second\n")
	iters = raw_input("Please enter the maximum number of iterations , if nothing is entered or entered value is not an integer, default is 40 times\n")
	print "Please enter your starting board for Conway's board of life. Enter each row at a time, represented by 0s and 1s separated by spaces. All rows must be the same length."
	while True:
		nextrow = raw_input("Please enter the a row. To finish the starting board just press enter.\n")
		if nextrow:
			row = nextrow.split()
			board.append(row)
		else:
			maingame = GameOfLife(board, tick, iters)
			if maingame.validateBoard():
				maingame.start()
			return


if __name__ == "__main__":
	print "Conway's Game of Life will continue to run for 1000 ticks or until the program is shut down via control C or other method"
	main()

	

