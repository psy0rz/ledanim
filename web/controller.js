$(document).ready(function()
{

    //strip_animator class and command array
    var strip_anim=new Module.strip_anim_c();

    // start ledstrip simulator via canvas
    function start_ledsim(){

        /////////////////////////////////// prepare canvas
        var MAX_LEDS=Module.MAX_LEDS;

        //fast pixel manipulation code borrowed from http://jsfiddle.net/andrewjbaker/Fnx2w/
        var canvas = document.getElementById('ledsim');
        var canvasWidth  = canvas.width;
        var canvasHeight = canvas.height;
        var ctx = canvas.getContext('2d');
        var imageData = ctx.getImageData(0, 0, canvasWidth, canvasHeight);

        var buf = new ArrayBuffer(imageData.data.length);
        var buf8 = new Uint8ClampedArray(buf);
        var data = new Uint32Array(buf);

        // Determine whether Uint32 is little- or big-endian.
        data[1] = 0x0a0b0c0d;

        var isLittleEndian = true;
        if (buf[4] === 0x0a && buf[5] === 0x0b && buf[6] === 0x0c && buf[7] === 0x0d)
        {
            isLittleEndian = false;
        }

        /// animate one step and update canvas
        function step()
        {
            requestAnimationFrame(step);
            strip_anim.step();


            var rgb;
            for (led=0;led<MAX_LEDS;led++)
            {
                rgb=strip_anim.get_led(led);
                // ctx.fillStyle = "rgb(" + rgb.r + "," + rgb.g + "," + rgb.b + ")";
                // ctx.fillRect(led*12, 10, 10,10);
                if (isLittleEndian) {
                    data[led] =
                    (255   << 24) |    // alpha
                    (rgb.b << 16) |    // blue
                    (rgb.g <<  8) |    // green
                    rgb.r;            // red
                } else {
                    data[led] =
                    (rgb.r << 24) |    // red
                    (rgb.g << 16) |    // green
                    (rgb.b <<  8) |    // blue
                    255;              // alpha
                }
            }

            imageData.data.set(buf8);
            ctx.putImageData(imageData, 0, 0);
        }
        step(); //start

    }

    //get current program from editor and compile it
    // var last_program="";
    function compile_editor(editor)
    {
        // if ($("#commands").val() != last_program)
        // {
        var lines=editor.getSession().getDocument().getAllLines();
        localStorage.setItem("commands", editor.getValue());
            // last_program=$("#commands").val();
        var commands=new Module.commands_t();
        var error=assemble_commands(lines, commands);
        if (error)
        {
            $("#compiler_msg").text(error[2]);
            $("#compiler_msg").addClass("error");

            editor.getSession().setAnnotations([{
                row: error[0]-1,
                column: 0,
                text: error[2],
                type: "error" // also warning and information
            }]);


        }
        else
        {
            $("#compiler_msg").text("Compiled ok.");
            $("#compiler_msg").removeClass("error");
            editor.getSession().clearAnnotations();
            strip_anim.set_commands(commands);
        }
        commands.delete();
        // }
    }

    //ace editor
    var editor = ace.edit("editor");
    editor.setTheme("ace/theme/twilight");
    // editor.session.setMode("ace/mode/javascript");

    // when user changes the program, recompile it after one second
    var wto;
    editor.on("change", function() {
     $("#compiler_msg").html("&nbsp;");
     $("#compiler_msg").removeClass("error");
     clearTimeout(wto);
     wto = setTimeout(function() {
         compile_editor(editor);
    }, 300);
    });

    if (localStorage.hasOwnProperty("commands"))
        editor.setValue(localStorage.getItem("commands"),1);
    compile_editor(editor);
    start_ledsim();
});
