CC = gcc
CFLAGS = -Wall -g
NAME_1 = client
NAME_2 = server
BUILD = build
SOURCE_CLIENT = src_client
SOURCE_SERVER = src_server

# --- Client build ---
TARGET_CLIENT = $(BUILD)/bin/$(NAME_1)
SRCS_CLIENT   = $(shell find $(SOURCE_CLIENT) -name "*.c")
OBJS_CLIENT   = $(patsubst $(SOURCE_CLIENT)/%.c, $(BUILD)/client/%.o, $(SRCS_CLIENT))

# --- Server build ---
TARGET_SERVER = $(BUILD)/bin/$(NAME_2)
SRCS_SERVER   = $(shell find $(SOURCE_SERVER) -name "*.c")
OBJS_SERVER   = $(patsubst $(SOURCE_SERVER)/%.c, $(BUILD)/server/%.o, $(SRCS_SERVER))

# Default: build both
all: $(TARGET_CLIENT) $(TARGET_SERVER)

# Link client
$(TARGET_CLIENT): $(OBJS_CLIENT)
	mkdir -p $(BUILD)/bin
	$(CC) $(CFLAGS) -o $@ $^

# Link server
$(TARGET_SERVER): $(OBJS_SERVER)
	mkdir -p $(BUILD)/bin
	$(CC) $(CFLAGS) -o $@ $^

# Compile client objects
$(BUILD)/client/%.o: $(SOURCE_CLIENT)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile server objects
$(BUILD)/server/%.o: $(SOURCE_SERVER)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)

run_client: $(TARGET_CLIENT)
	@$(TARGET_CLIENT)

run_server: $(TARGET_SERVER)
	@$(TARGET_SERVER)

.PHONY: all clean run_client run_server
