#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/inotify.h>


static void 
PrintEvent(const char *base, struct inotify_event *event)
{
    char *operate;
    int mask = event->mask;

    if (mask & IN_ACCESS) operate = "ACCESS";
    if (mask & IN_ATTRIB) operate = "ATTRIB";
    if (mask & IN_CLOSE_WRITE) operate = "CLOSE_WRITE";
    if (mask & IN_CLOSE_NOWRITE) operate = "CLOSE_NOWRITE";
    if (mask & IN_CREATE) operate = "CREATE";
    if (mask & IN_DELETE_SELF) operate = "DELETE_SELF";
    if (mask & IN_MODIFY) operate = "MODIFY";
    if (mask & IN_MOVE_SELF) operate = "MOVE_SELF";
    if (mask & IN_MOVED_FROM) operate = "MOVED_FROM";
    if (mask & IN_MOVED_TO) operate = "MOVED_TO";
    if (mask & IN_OPEN) operate = "OPEN";
    if (mask & IN_IGNORED) operate = "IGNORED";
    if (mask & IN_DELETE) operate = "DELETE";
    if (mask & IN_UNMOUNT) operate = "UNMOUNT";

    printf("%s: %s\n", base, operate);
}


int main()
{
    char buf[BUFSIZ] = {0};
    int inotifyfd = inotify_init();
    int wd = inotify_add_watch(inotifyfd, "/root/tsh-test/inotify/", IN_ALL_EVENTS);
    char *p = NULL; 
    while (1)
    {
        memset(buf, 0, BUFSIZ);
        ssize_t nread = read(inotifyfd, buf, BUFSIZ);
        if (nread <= 0 ) {
            continue;
        }
        
        int offset = 0;
        struct inotify_event event;
        do {
            memset(&event, 0x00, sizeof(event));
            memcpy(&event, &buf[offset], sizeof(event));
            if (event.len > 0) {
                printf("name: %s", event.name);
            }
                printf("len: %d", event.len);
            PrintEvent(event.name, &event);
            offset += sizeof(struct inotify_event) + event.len;
        } while (offset < nread);
    }
    
    return 0;
}