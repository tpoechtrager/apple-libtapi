BUILD_API_ANALYZER = $(shell find $(SRC) -name build_api_analyzer)

api-analyzer: $(OBJROOT) $(SYMROOT) $(DSTROOT)
	cd $(OBJROOT) && \
		$(BUILD_API_ANALYZER) $(ENABLE_ASSERTIONS) $(LLVM_OPTIMIZED)
