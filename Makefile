# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  Cathedral_config = debug
  ct_config = debug

else ifeq ($(config),release)
  Cathedral_config = release
  ct_config = release

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := Cathedral ct

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

Cathedral:
ifneq (,$(Cathedral_config))
	@echo "==== Building Cathedral ($(Cathedral_config)) ===="
	@${MAKE} --no-print-directory -C . -f Cathedral.make config=$(Cathedral_config)
endif

ct: Cathedral
ifneq (,$(ct_config))
	@echo "==== Building ct ($(ct_config)) ===="
	@${MAKE} --no-print-directory -C . -f ct.make config=$(ct_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f Cathedral.make clean
	@${MAKE} --no-print-directory -C . -f ct.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   Cathedral"
	@echo "   ct"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"