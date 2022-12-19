/* Standard Linux headers */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fb.h>

/* Davinci specific kernel headers */
#include <video/davincifb_ioctl.h>

/* Demo headers */
#include "interface.h"

/* The levels of initialization */
#define SCREENINITIALIZED   0x1

/* Video window to show diagram on */
#define FBVID_DEVICE        "/dev/fb/3"
#define OSD_DEVICE          "/dev/fb/0"
#define ATTR_DEVICE         "/dev/fb/2"

#define UYVY_BLACK          0x10801080

#define D1RESOLUTIONSTRINGPAL   "720x576"
#define D1RESOLUTIONSTRINGNTSC  "720x480"
#define CIFRESOLUTIONSTRINGPAL  "352x288"
#define CIFRESOLUTIONSTRINGNTSC "352x240"

/* Add argument number x of string y */
#define addArg(x, y)                     \
    argv[(x)] = malloc(strlen((y)) + 1); \
    if (argv[(x)] == NULL)               \
        return FAILURE;                  \
    strcpy(argv[(x)++], (y))

enum StartLevels {
    START_BEGINNING,
    START_MENU,
    START_ENCODEDECODE,
    START_ENCODE,
    START_DECODE,
    NUM_START_LEVELS
};

/* Global variable declarations for this application */
GlobalData gbl = { NULL, NOSTD };

static enum StartLevels startLevel = START_BEGINNING;
static int launch = FALSE;

/******************************************************************************
 * drawDiagram
 ******************************************************************************/
int drawDiagram(char *diagram)
{
    struct fb_var_screeninfo varInfo;
    struct fb_fix_screeninfo finfo;
    int                      fd;
    int                      size;
    int                      i;
    int                      fileFd;
    int                      numBytes;
    char                    *src, *srcPtr;
    char                    *dst, *dstPtr;
    int                      lineSize;

    fd = open(FBVID_DEVICE, O_RDWR);

    if (fd == -1) {
        ERR("Failed to open fb device %s (%s)\n", FBVID_DEVICE,
                                                  strerror(errno));
        return FAILURE;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &varInfo) == -1) {
        ERR("Failed FBIOGET_VSCREENINFO on %s (%s)\n", FBVID_DEVICE,
                                                       strerror(errno));
        return FAILURE;
    }

    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        ERR("Failed FBIOGET_FSCREENINFO on %s (%s)\n", FBVID_DEVICE,
                                                       strerror(errno));
        return FAILURE;
    }
   
    varInfo.yoffset = 0;

    /* Swap the working buffer for the displayed buffer */
    if (ioctl(fd, FBIOPAN_DISPLAY, &varInfo) == -1) {
        ERR("Failed FBIOPAN_DISPLAY (%s)\n", strerror(errno));
        return FAILURE;
    }

    dst = (char *) mmap (NULL,
                         varInfo.yres_virtual * finfo.line_length,
                         PROT_READ | PROT_WRITE,
                         MAP_SHARED,
                         fd, 0);

    /* Map the video buffer to user space */
    if (dst == MAP_FAILED) {
        ERR("Failed mmap on %s (%s)\n", FBVID_DEVICE, strerror(errno));
        return FAILURE;
    }

    varInfo.xres = SCREEN_WIDTH;
    varInfo.yres = SCREEN_HEIGHT;
    varInfo.yres_virtual = NUM_OSD_BUFS * SCREEN_HEIGHT;
    varInfo.bits_per_pixel = SCREEN_BPP;

    /* Set video display format */
    if (ioctl(fd, FBIOPUT_VSCREENINFO, &varInfo) == -1) {
        ERR("Failed FBIOPUT_VSCREENINFO on %s (%s)\n", FBVID_DEVICE,
                                                       strerror(errno));
        return FAILURE;
    }

    if (varInfo.xres != SCREEN_WIDTH ||
        varInfo.yres != SCREEN_HEIGHT ||
        varInfo.bits_per_pixel != SCREEN_BPP) {
        ERR("Failed to get the requested screen size: %dx%d at %d bpp\n",
            SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
        return FAILURE;
    }

    size = varInfo.xres * varInfo.yres * varInfo.bits_per_pixel / 8;
    fileFd = open(diagram, O_RDONLY);

    if (fileFd == -1) {
        ERR("Failed to open demo diagram file (%s)\n", strerror(errno));
        return FAILURE;
    }

    src = malloc(size);

    if (src == NULL) {
        ERR("Failed to allocate memory for diagram\n");
        return FAILURE;
    }

    numBytes = read(fileFd, src, size);

    if (numBytes != size) {
        ERR("Error reading data from diagram file (%s)\n", strerror(errno));
        return FAILURE;
    }

    srcPtr = src;
    dstPtr = dst;
    lineSize = (varInfo.xres * varInfo.bits_per_pixel) / 8;
    for (i = 0; i < varInfo.yres; i++) {
        memcpy(dstPtr, srcPtr, lineSize);
        srcPtr += lineSize;
        dstPtr += finfo.line_length;
    }

    free(src);

    munmap(dst, varInfo.yres_virtual * finfo.line_length);
    close(fd);
    close(fileFd);

    return SUCCESS;
}

/******************************************************************************
 * screenInit
 ******************************************************************************/
static int screenInit(void)
{
    struct fb_var_screeninfo varInfo;
    int                      fd;
    unsigned short          *display;
    FILE * fp;
    char mode[16];
    int status = FAILURE;

    fd = open(OSD_DEVICE, O_RDWR);

    if (fd == -1) {
        ERR("Failed to open fb device %s\n", OSD_DEVICE);
        return FAILURE;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &varInfo) == -1) {
        ERR("Failed ioctl FBIOGET_VSCREENINFO on %s\n", OSD_DEVICE);
        return FAILURE;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &varInfo) == -1) {
        ERR("Failed ioctl FBIOGET_VSCREENINFO on %s\n", OSD_DEVICE);
        return FAILURE;
    }

    if ((fp = fopen("/sys/class/davinci_display/ch0/mode", "r")) != NULL) {
        fscanf(fp, "%s", mode);
        fclose(fp);
        if (strcmp(mode, "NTSC") == 0) {
            DBG("NTSC selected\n");
     gbl.yFactor = NTSCSTD;
            status = SUCCESS;
        }
        else if (strcmp(mode, "PAL") == 0) {
            DBG("PAL selected\n");
            gbl.yFactor = PALSTD;
            status = SUCCESS;
        }
    }

    /* Try the requested size */
    varInfo.xres = SCREEN_WIDTH;
    varInfo.yres = SCREEN_HEIGHT;
    varInfo.yres_virtual = NUM_OSD_BUFS * SCREEN_HEIGHT;
    varInfo.bits_per_pixel = SCREEN_BPP;

    if (ioctl(fd, FBIOPUT_VSCREENINFO, &varInfo) == -1) {
        ERR("Failed ioctl FBIOPUT_VSCREENINFO on %s\n", OSD_DEVICE);
        return FAILURE;
    }

    if (varInfo.xres != SCREEN_WIDTH ||
        varInfo.yres != SCREEN_HEIGHT ||
        varInfo.bits_per_pixel != SCREEN_BPP) {
        ERR("Failed to get the requested screen size: %dx%d at %d bpp\n",
            SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
    }

    display = (unsigned short *) mmap(NULL, SCREEN_SIZE, PROT_READ | PROT_WRITE,
                                      MAP_SHARED, fd, 0);

    if (display == MAP_FAILED) {
        ERR("Failed mmap on %s\n", OSD_DEVICE);
        return FAILURE;
    }

    gbl.display = display;

    return fd;
}

/******************************************************************************
 * setOsdTransparency
 ******************************************************************************/
int setOsdTransparency(unsigned char trans)
{
    struct fb_var_screeninfo varInfo;
    struct fb_fix_screeninfo fixInfo;
    unsigned short          *attrDisplay;
    int                      attrSize;
    int                      fd;

    /* Open the attribute device */
    fd = open(ATTR_DEVICE, O_RDWR);

    if (fd == -1) {
        ERR("Failed to open attribute window %s\n", ATTR_DEVICE);
        return FAILURE;
    }

    if (ioctl(fd, FBIOGET_FSCREENINFO, &fixInfo) == -1) {
        ERR("Failed FBIOGET_FSCREENINFO on %s (%s)\n", FBVID_DEVICE,
                                                       strerror(errno));
        return FAILURE;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &varInfo) == -1) {
        ERR("Error reading variable information.\n");
        return FAILURE;
    }

    attrSize = fixInfo.line_length * varInfo.yres;

    /* Map the attribute window to this user space process */
    attrDisplay = (unsigned short *) mmap(NULL, attrSize,
                                          PROT_READ | PROT_WRITE,
                                          MAP_SHARED, fd, 0);
    if (attrDisplay == MAP_FAILED) {
        ERR("Failed mmap on %s\n", ATTR_DEVICE);
        return FAILURE;
    }

    /* Fill the window with the new attribute value */
    memset(attrDisplay, trans, attrSize);

    munmap(attrDisplay, attrSize);
    close(fd);

    return SUCCESS;
}

/******************************************************************************
 * launchDemo
 ******************************************************************************/
int launchDemo(DemoEnv *envp)
{
    char *argv[10];
    char *extension;
    int  i = 0;

    /* Note that the malloc:s are freed up by execv */
    switch (envp->demoSelect) {
        /* Launch encodedecode demo */
        case ENCDEC:
            addArg(i, "./encodedecode");
            addArg(i, "-i");
            addArg(i, "-d");
            addArg(i, "-b");
//            addArg(i, "3000000");
            addArg(i, "6000000");

            argv[i] = NULL;

            if (execv("./encodedecode", argv) == -1) {
                return FAILURE;
            }
            break;

        /* Launch encode demo */
        case ENC:
            addArg(i, "./encode");
            addArg(i, "-i");
            addArg(i, "-t");
            addArg(i, "300");
            addArg(i, "-v");

            if (envp->videoAlg == MPEG4) {
                addArg(i, "data/videos/demo.mpeg4");
                addArg(i, "-s");
                addArg(i, "data/sounds/demompeg4.g711");
            }
            else {
                addArg(i, "data/videos/demo.264");
                addArg(i, "-s");
                addArg(i, "data/sounds/demo264.g711");
            }

            addArg(i, "-b");

            if (envp->videoBps == CONSTANT && envp->videoAlg == MPEG4) {
                addArg(i, "6000000");
            }
            else if (envp->videoBps == CONSTANT && envp->videoAlg == H264) {
                addArg(i, "5000000");
            }
            else {
                addArg(i, "-1");
            }

            addArg(i, "-r");

            if (gbl.yFactor == NTSCSTD) {
                addArg(i, D1RESOLUTIONSTRINGNTSC);
            }
            else {
                addArg(i, D1RESOLUTIONSTRINGPAL);
            }

            argv[i] = NULL;

            if (execv("./encode", argv) == -1) {
                return FAILURE;
            }
            break;

        /* Launch decode demo */
        case DEC:
            addArg(i, "./decode");
            addArg(i, "-i");
            addArg(i, "-v");
            addArg(i, envp->videoFile);

            extension = rindex(envp->soundFile, '.');
            if (extension == NULL) { // shouldn't happen
                ERR("Sound file without extension (%s)\n", envp->soundFile);
                return FAILURE;
            }

            if (strcmp(extension, ".g711") == 0) {
                addArg(i, "-s");
            }
            else {
                addArg(i, "-a");
            }

            addArg(i, envp->soundFile);
            addArg(i, "-l");

            argv[i] = NULL;

            if (execv("./decode", argv) == -1) {
                return FAILURE;
            }
            break;

        /* Launch third party demo */
        case THIRDPARTY:
            if (chdir(envp->thirdPartyCmdPath) == 0) {
                if (execl("app.sh", NULL) == -1) {
                    return FAILURE;
                }
            }
            else {
                ERR("Failed to chdir to %s\n", envp->thirdPartyCmdPath);
            }
            break;
    }

    return SUCCESS; // Should never reach
}

/******************************************************************************
 * showDemoInterface
 ******************************************************************************/
int showDemoInterface(DemoEnv *envp, int *quitPtr)
{
    int ret = SUCCESS;

    switch (envp->demoSelect) {
        case ENCDEC:
            ret = encodeDecodeFxn(envp);

            if (ret == SUCCESS) {
                *quitPtr = TRUE;
                launch = TRUE;
            }
            break;
        case ENC:
            ret = encodeFxn(envp);

            if (ret == SUCCESS) {
                *quitPtr = TRUE;
                launch = TRUE;
            }
            break;
        case DEC:
            ret = decodeFxn(envp);

            if (ret == SUCCESS) {
                *quitPtr = TRUE;
                launch = TRUE;
            }
            break;
        case THIRDPARTY:
            ret = thirdPartyFxn(envp);

            if (ret == SUCCESS) {
                *quitPtr = TRUE;
                launch = TRUE;
            }
            break;
    }

    return ret;
}

/******************************************************************************
 * usage
 ******************************************************************************/
static void usage(void)
{
    printf("Usage: interface [options]\n\n"
           "Options:\n"
           "-l | --level   Level to start interface at (0-4) [0]\n"
           "-h | --help    Print this message\n\n"
           "Levels available:\n"
           " 0 - Start at the beginning [default]\n"
           " 1 - Skip the initial remote control help screen\n"
           " 2 - Go directly to the encodedecode demo interface\n"
           " 3 - Go directly to the encode demo interface\n"
           " 4 - Go directly to the decode demo interface\n\n");
}

/******************************************************************************
 * parseArgs
 ******************************************************************************/
static void parseArgs(int argc, char *argv[], DemoEnv *envp)
{
    const char shortOptions[] = "l:h";
    const struct option longOptions[] = {
        {"level", required_argument, NULL, 'l'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };
    int     index;
    int     c;

    for (;;) {
        c = getopt_long(argc, argv, shortOptions, longOptions, &index);

        if (c == -1) {
            break;
        }

        switch (c) {
            case 0:
                break;

            case 'l':
                startLevel = atoi(optarg);
                break;

            case 'h':
                usage();
                exit(EXIT_SUCCESS);

            default:
                usage();
                exit(EXIT_FAILURE);
        }
    }
}

/******************************************************************************
 * main
 ******************************************************************************/
int main(int argc, char *argv[])
{
    unsigned int    initMask  = 0;
    int             status    = EXIT_SUCCESS;
    int             ret       = SUCCESS;
    int             quit      = 0;
    int             screenFd;
    DemoEnv         env;

    /* Parse the arguments given to the app and set the app environment */
    parseArgs(argc, argv, &env);

    printf("Demo interface started at level %d.\n", startLevel);

    screenFd = screenInit();

    if (screenFd == -1) {
        cleanup(EXIT_FAILURE);
    }

    if (setOsdTransparency(0x77) == -1) {
        cleanup(EXIT_FAILURE);
    }

    initMask |= SCREENINITIALIZED;

    /* In which state to start the interface */
    switch (startLevel) {
        case START_BEGINNING:
            ret = startupFxn(&env);

            if (ret == FAILURE) {
                cleanup(EXIT_FAILURE);
            }

        case START_MENU:
            /* Let the user select a demo */
            ret = menuFxn(&env);

            if (ret == FAILURE) {
                cleanup(EXIT_FAILURE);
            }

            if (ret == NOSELECTION) {
                cleanup(EXIT_SUCCESS);
            }
            break;

        case START_ENCODEDECODE:
            env.demoSelect = ENCDEC;
            break;

        case START_ENCODE:
            env.demoSelect = ENC;
            break;

        case START_DECODE:
            env.demoSelect = DEC;
            break;

        default:
            fprintf(stderr, "Only start level 0-%d supported\n",
                    NUM_START_LEVELS - 1);
            cleanup(EXIT_FAILURE);
            break;
    }

    while (!quit) {
        /* Show the selected demo interface */
        ret = showDemoInterface(&env, &quit);

        if (ret == FAILURE) {
            breakLoop(EXIT_FAILURE);
        }

        if (ret == PLAYBACK) {
            env.demoSelect = DEC;
            if (showDemoInterface(&env, &quit) == FAILURE) {
                breakLoop(EXIT_FAILURE);
            }

        }

        if (quit) {
            breakLoop(EXIT_SUCCESS);
        }

        /* No transparency on main menu */
        if (setOsdTransparency(0x77) == -1) {
            breakLoop(EXIT_FAILURE);
        }

        /* Let the user select a demo */
        ret = menuFxn(&env);

        if (ret == FAILURE) {
            breakLoop(EXIT_FAILURE);
        }

        if (ret == NOSELECTION) {
            breakLoop(EXIT_SUCCESS);
        }
    }

cleanup:
    if (initMask & SCREENINITIALIZED) {
        close(screenFd);
    }

    if (status == EXIT_SUCCESS && launch && launchDemo(&env) == FAILURE) {
        status = FAILURE;
    }

    exit(status);
}
