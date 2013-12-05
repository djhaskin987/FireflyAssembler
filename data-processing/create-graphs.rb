require 'bio'

def stats file
	lengths = []
	n50 = 0
	File.open file do |f|
		ff = Bio::FlatFile.new(Bio::FastaFormat,f)
		ff.each_entry do |record|
			lengths.push record.length
		end
	end
	lengths.sort!
	total_length = lengths.inject(:+)
	lengths_so_far = 0
	lengths.each do |l|
		lengths_so_far += l
		if lengths_so_far > total_length/2
			n50 = l
			break
		end
	end
	[lengths.size, n50]
end

rows = []
(500..5000).step(500) do |count|
	row = [count]
	# puts row.join(",")
	["greedy", "localsearch", "firefly"].each do |algo|
		Dir.glob("./**/*#{algo}_#{count}.fasta") do |f|
			# puts f
			row.push stats(f)
			# puts contigs
		end
		done = false
		Dir.glob("./**/*#{algo}_#{count}.out") do |f|
			unless done
				time = File.open(f).readlines[-3][27..1000].split.first
				row.push time
			end
			done = true
		end
	end
	rows.push row
end

File.open("comparisons.csv", "w") do |f|
	titles = ["Count"]
	["Greedy", "LocalSearch", "Firefly"].each do |algo|
		["Contigs", "N50", "Time"].each do |rating|
			titles.push "#{algo} #{rating}"
		end
	end

	rows.unshift titles
	rows.each do |row|
		f.puts row.join(",")
		puts row.join ","
	end
end