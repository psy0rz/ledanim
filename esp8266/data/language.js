param_def=
{
    "uint8_t": [
        "Number"
    ]


}


command_def=
{
    "eof": [
        0,
        "End of program",
        []
    ],
    "update": [
        1,
        "Update led strip with current values",
        [],

    "delay": [
        3,
        "Update led strip and delay for this many steps.",
        [ param_def.uint16_t ],
    ]

CMD_DELAY_8,      //delay execution, but keep updating ledstrip. (doing fades and stuff)   8 bits delay, in steps
CMD_DELAY_16,     //delay execution, but keep updating ledstrip. (doing fades and stuff)  16 bits delay. in steps

CMD_REPEAT_BEGIN,
CMD_REPEAT_BEGIN_RND,
CMD_REPEAT_END,

CMD_LED_NR_8,
CMD_LED_NR_8_RND,
CMD_LED_NR_16,
CMD_LED_NR_16_RND,
CMD_LED_SET_NEXT,
CMD_PEN_COLOR,
CMD_PEN_COLOR_RND,
CMD_PEN_STEP,
CMD_PEN_WIDTH,
CMD_PEN_WIDTH_RND,
CMD_PEN_DRAW,
CMD_PEN_FADE_MODE,
CMD_PEN_FADE_SPEED,
CMD_PEN_FADE_SPEED_RND,
CMD_PEN_CLONE_COUNT,
CMD_PEN_CLONE_OFFSET,

}
