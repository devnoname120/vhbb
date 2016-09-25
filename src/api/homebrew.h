#pragma once

typedef struct {
	char *UUID;
	char *name;
	char *developer;
	char *type;
	char *first_release;
	char *description;
} Homebrew;

typedef struct {
	char *url;
} Icon;

typedef struct {
	char *UUID;
	char *url;
	char *description;
} Screenshot;