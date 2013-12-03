#!/usr/bin/python

# Usage: python sim-reads.py input_file.fa output_file.fa [coverage]

import sys, random
from Bio import SeqIO, Seq

DEFAULT_COVERAGE = 5
AVERAGE_LENGTH = 100

def produce_reads(in_file, out_file, coverage):
    input_sequence = SeqIO.read(in_file,"fasta").seq.tostring()
    # print input_sequence.seq.tostring()

    total_length = coverage*len(input_sequence)

    length_so_far = 0
    reads = []

    while length_so_far < total_length:
        length = random.randint(AVERAGE_LENGTH*.75, AVERAGE_LENGTH*1.25)
        length_so_far += length
        start = random.randrange(len(input_sequence))
        seq = ''
        if start+length > len(input_sequence):
            seq = input_sequence[start:]
        else:
            seq = input_sequence[start:start+length]
        reads.append(SeqIO.SeqRecord(Seq.Seq(seq), id=str(start), description="length %i"%(len(seq))))

    with open(out_file,"w") as f:
        SeqIO.write(iter(reads), f, "fasta")


if len(sys.argv) == 1:
    produce_reads("./sequences-3.fasta", "./sequences-3.fasta.out", DEFAULT_COVERAGE)

elif len(sys.argv) == 3:
    produce_reads(sys.argv[1], sys.argv[2], DEFAULT_COVERAGE)

elif len(sys.argv) == 4:
    produce_reads(sys.argv[1], sys.argv[2], int(sys.argv[3]))

else:
    print "Usage: python sim-reads.py input_file.fa output_file.fa [coverage]"

