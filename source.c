
#define MEM_SIZE 1024*1024*32 // 32 MiB
#define MAX_ENTRIES 1024*1024 // avg. 32 bytes per entry
// this configuration should need 52 MiB in total

static char mem[MEM_SIZE];

typedef struct {
	int index;
	int size;
	char free;
	char memchk;
} memEntry;

static memEntry entries[MAX_ENTRIES];
static memEntry swap[MAX_ENTRIES];
static int entryCount = 0;

void mem_init() {
	entries[0].index = 0;
	entries[0].size = MEM_SIZE;
	entries[0].free = 1;
	entryCount = 1;
}
void* mem_alloc(int size) {
	if (entryCount >= MAX_ENTRIES) return 0;

	int entry = -1;
	for (int i = 0; i < entryCount && entry == -1; i++) {
		if (entries[i].free != 0 && entries[i].size >= size) {
			entry = i;
		}
	}
	if (entry == -1) return 0;
	else {
		if (entries[entry].size != size) {
			entries[entryCount].free = 1;
			entries[entryCount].size = entries[entry].size - size;
			entries[entryCount].index = entries[entry].index + size;
			entryCount++;
		}
		entries[entry].free = 0;
		entries[entry].size = size;
		return entries[entry].index + mem;
	}
}
void* mem_alloc_smallest_fitting(int size){
	if (entryCount >= MAX_ENTRIES) return 0;

	int entry = -1;
	for (int i = 0; i < entryCount && entry == -1; i++) {
		if (entries[i].free != 0 && entries[i].size >= size) {
			if(entry == -1) entry = i;
			else if (entries[i].size < entries[entry].size) entry = i;
		}
	}
	if (entry == -1) return 0;
	else {
		if (entries[entry].size != size) {
			entries[entryCount].free = 1;
			entries[entryCount].size = entries[entry].size - size;
			entries[entryCount].index = entries[entry].index + size;
			entryCount++;
		}
		entries[entry].free = 0;
		entries[entry].size = size;
		return entries[entry].index + mem;
	}
}
void mem_free(void* ptr) {
	int index = (char*)ptr - mem; // index in mem[]

	for(int i=0; i<entryCount; i++){
        if(entries[i].index == index){
            entries[i].free = 1;
            return;
        }
    }
}
void mem_defrag() {
	int newEntryCount = 0;

	for(int i=0; i<entryCount; i++){
		if(entries[i].memchk == 0){
			swap[newEntryCount].free = entries[i].free;
			swap[newEntryCount].index = entries[i].index;
			swap[newEntryCount].size = entries[i].size;
			swap[newEntryCount].memchk = 0;
			entries[i].memchk=1;
			for(int j=i+1; j<entryCount; j++){
				if(entries[j].index == entries[i].index+entries[i].size){
					if(entries[j].free != 0 && entries[i].free != 0){
						swap[newEntryCount].size += entries[j].size;
						entries[j].memchk=1;
						j = i + 1; // start over
					}
					else{
						j = entryCount + 16; // next i
					}
				}
			}
			newEntryCount++;
		}
	}

	// swap <-> entries
	memEntry* eptr = entries;
	*entries = *swap;
	*swap = *eptr;
	entryCount = newEntryCount;
}
void mem_info(int* bytesTotal, int* bytesFree, int* fragments, int* fragmentsFree){
	*bytesTotal = 0;
	*bytesFree = 0;
	*fragments = 0;
	*fragmentsFree = 0;

	for(int i=0; i<entryCount; i++){
		*bytesTotal += entries[i].size;
		*fragments += 1;
		if(entries[i].free){
			*fragmentsFree += 1;
			*bytesFree+=entries[i].size;
		}
	}
}
