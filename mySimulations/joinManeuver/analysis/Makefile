# tool for indexing vec files
SCAVETOOL = scavetool
# scripts location
SCRIPTDIR = .
# results location
RESDIR = ../results
# script for merging
MERGESCRIPT = $(SCRIPTDIR)/merge.R

# match all .vec files for the MyJoinManeuver config
MYJOINMANEUVER      = $(wildcard $(RESDIR)/MyJoinManeuver*.vec)
# change suffix from .vec to .Rdata and add the join prefix
MYJOINMANEUVER_DATA = $(MYJOINMANEUVER:$(RESDIR)/%.vec=$(RESDIR)/join.%.Rdata)

# vector index files and Rdata files
VCI = $(VECTOR:%.vec=%.vci)
RDATA = $(VECTOR:%.vec=%.Rdata)
CSV = $(VECTOR:%.vec=%.csv)

# all make targets
all: MyJoinManeuver.Rdata 

# to make MyJoinManeuver.Rdata we need to merge all files starting with join.MyJoinManeuver
# before this, check that all MYJOINMANEUVER_DATA files have been processed
$(RESDIR)/MyJoinManeuver.Rdata: $(MYJOINMANEUVER_DATA)
	Rscript $(MERGESCRIPT) $(RESDIR)/ join.MyJoinManeuver $(notdir $@) map-config myJoin Rdata
MyJoinManeuver.Rdata: $(RESDIR)/MyJoinManeuver.Rdata

# to make all join.*.Rdata files we need to run the generic parser
join.%.Rdata: %.vec %.vci
	Rscript generic-parser.R $< map-config myJoin join Rdata

# if vec files are not indexed, index them
%.vci : %.vec
	$(SCAVETOOL) index $<

# helper to print variable values. e.g.: make print-DELAY_DATA
print-%:
	@echo '$*=$($*)'

# every intermediate file is kept instead of being automatically deleted.
# .vci files are detected as intermediate and thus cancelled when the make
# command terminates. however, such files can still be needed by other
# targets, so keep them instead of re-doing indexing
.SECONDARY :
