require 'graphviz'

class String
  def overlap other
    str = self.dup
    (0...self.size).each do |i|
      return i if self[i..size] == other[0...size-i]
      return other.size if self.include? other
    end
    return 0
  end
end

reads = Array.new
File.open('../resources/sequences-3.fasta.out') do |f|
  f.each_line do |line|
    reads.push line if ['A', 'C', 'T', 'G'].include? line[0] and not reads.include? line
  end
end

g = GraphViz.new( :G, :type => :digraph )

reads.each_with_index do |r1, i|
  reads.each_with_index do |r2, j|
    if r1.overlap(r2) > 3
      g.add_nodes(i.to_s, label: reads[i])
      g.add_nodes(j.to_s, label: reads[j])
      g.add_edges(i.to_s, j.to_s, label: r1.overlap(r2))
    end
  end
end

# Generate output image
g.output( :png => "example_graph.png" )
