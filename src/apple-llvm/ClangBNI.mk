##
# Clang B&I Build Logic
##

# This makefile defines the common build logic which is shared by all the Clang
# projects. Projects are expected to define the following variables and then
# include this Makefile::
#
#   Clang_Use_Assertions := {0, 1}
#     Enable/disable asserts.
#

# Particular build targets (which correspond to project configurations) may
# override various defaults based on what makes sense to build/install for that
# particular target.

##
# Default target.

all: help

help:
	@echo "usage: make [{VARIABLE=VALUE}*] <target>"
	@echo
	@echo "The Apple Clang makefile is primarily intended for use with XBS."
	@echo
	@echo "Supported B&I related targets are:"
	@echo "  installsrc    -- Copy source files from the current" \
	      "directory to the SRCROOT."
	@echo "  clean         -- Does nothing, just for XBS support."
	@echo "  installhdrs   -- Does nothing, just for XBS support."
	@echo "  install       -- Alias for install-clang."
	@echo "  install-clang -- Build the Apple Clang compiler."
	@echo "  install-cross -- Build the Apple Clang compiler, for ARM."
	@echo "  install-clang-links"
	@echo "                -- Install links from a platforms subdirectory" \
	      "to the root clang."
	@echo
	@echo "The Makefile also supports the following utility targets:"
	@echo "  commit-buildbot-order-file -- order file update utility"
	@echo "    This command is for use with buildbot, it copies back a"
	@echo "    generated order file to the "

MKDIR		:= /bin/mkdir -p -m 0755

# Default is to build Clang.
install: clang
install-cross: clang

# We install the source using rsync. We take particular care to:
#
#   1. Exclude any source control files.
#
#   2. Exclude any editor or OS cruft.
#
#   3. Exclude any build products that might be expected to be in a users local
#      tree (e.g., docs/_build) but not in the build directory. We don't try and
#      support users silly enough to do in-tree builds, though.
#
#   4. Exclude all tests.
#
#      This is unfortunate -- we would ideally like to have the exact versions
#      of the tests present at a submission archived -- but XBS does not deal
#      well with projects with many small files and the copy to the submission
#      server takes a very long time without this.
#
#   5. Copy "unsafe" links (links to outside the input directory). This allows
#      developers to link to a properly set up LLVM source directory from
#      Clang-IB and still be able to run buildit or submitproject directly.
installsrc:
	@echo "Installing source..."
	$(_v) $(MKDIR) "$(SRCROOT)"
	$(_v) time rsync -ar . "$(SRCROOT)" \
	  --exclude .git --exclude .svn \
	  --exclude .DS_Store --exclude '*~' --exclude '.*~' \
	  --exclude src/docs/_build \
	  --exclude src/test/ --exclude src/tools/clang/test/ \
	    --exclude 'src/projects/*/test/' \
	  --copy-unsafe-links

# The clean target is run after installing sources, but we do nothing because
# the expectation is that we will just avoid copying in cruft during the installsrc
# phase.
clean:

# We do not need to do anything for the install headers phase.
installhdrs:

