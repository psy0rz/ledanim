//wait until emscripten is ready:
Module={};
Module['onRuntimeInitialized']=function()
{

    //wait until document is ready:
    $( document ).ready( function()
    {

        //strip_animator class and command array
        var strip_anim=new Module.strip_anim_c();
        var MAX_LEDS=Module.MAX_LEDS;



        // start ledstrip simulator via canvas
        function init_canvas(){

            /////////////////////////////////// prepare canvas

            //fast pixel manipulation code borrowed from http://jsfiddle.net/andrewjbaker/Fnx2w/
            var canvas = document.getElementById('ledsim');
            var canvasWidth  = canvas.width;
            var canvasHeight = canvas.height;
            canvas_context = canvas.getContext('2d');
            image_data = canvas_context.getImageData(0, 0, canvasWidth, canvasHeight);

            var image_buf = new ArrayBuffer(image_data.data.length);
            image_buf8 = new Uint8ClampedArray(image_buf);
            image_data32 = new Uint32Array(image_buf);

            // Determine whether Uint32 is little- or big-endian.
            image_data32[1] = 0x0a0b0c0d;

            is_little_endian = true;
            if (image_buf[4] === 0x0a && image_buf[5] === 0x0b && image_buf[6] === 0x0c && image_buf[7] === 0x0d)
            {
                is_little_endian = false;
            }


        }


        /// animate one step and update canvas
        function step()
        {
            requestAnimationFrame(step);
            strip_anim.step();


            var rgb;
            if (is_little_endian)
            {
                //optimized a bit to make loop cheaper
                for (led=0;led<leds;led++)
                {
                    rgb=strip_anim.get_led(led);
                    image_data32[led] =
                    (255   << 24) |    // alpha
                    (rgb.b << 16) |    // blue
                    (rgb.g <<  8) |    // green
                    rgb.r;            // red
                }
            }
            else
            {
                for (led=0;led<leds;led++)
                {
                    image_data32[led] =
                    (rgb.r << 24) |    // red
                    (rgb.g << 16) |    // green
                    (rgb.b <<  8) |    // blue
                    255;              // alpha
                }
            }

            image_data.data.set(image_buf8);
            canvas_context.putImageData(image_data, 0, 0);
        }


        //get current program from editor and compile it
        // var last_program="";
        function compile_editor(editor)
        {


            localStorage.setItem("current_program", editor.getValue());
            localStorage.setItem("current_program_name", $("#program_name").val());


            try
            {
                pen.clear_commmands();
                eval(editor.getValue());
                error="";
            }
            catch(e)
            {
                error=String(e);
            }

            if (error)
            {
                $("#compiler_msg").text(error);
                $("#compiler_msg").addClass("error");

            }
            else
            {
                $("#compiler_msg").text("Compiled ok, "+pen.commands.size()+" bytes.");
                $("#compiler_msg").removeClass("error");
                strip_anim.set_commands(pen.commands);
            }
        }

        //download a string as a textfile
        //from http://stackoverflow.com/questions/3665115/create-a-file-in-memory-for-user-to-download-not-through-server
        function download(filename, data) {
            var blob = new Blob([data], {type: 'text/plain'});
            if(window.navigator.msSaveOrOpenBlob) {
                window.navigator.msSaveBlob(blob, filename);
            }
            else{
                var elem = window.document.createElement('a');
                elem.href = window.URL.createObjectURL(blob);
                elem.download = filename;
                document.body.appendChild(elem);
                elem.click();
                document.body.removeChild(elem);
            }
        }

        function update_quickload_list()
        {
            $(".quick_load").remove();
            $.each(Object.keys(localStorage), function(nr, key)
            {
                if (key.match("^program "))
                {
                    var clone=$(".quick_load_template").clone();
                    clone.removeClass("quick_load_template");
                    clone.addClass("quick_load");
                    clone.insertAfter($(".quick_load_template")).text(key.replace(/^program /, ""));
                }
            });
            // localStorage.setItem("program "+$("#program_name").val(), editor.getValue());

        }

        var timeout;
        function delayed(func)
        {
            clearTimeout(timeout);
            timeout = setTimeout(func, 300);
        }

        //// INITIALISATION

        //ace editor
        var editor = ace.edit("editor");
        editor.setTheme("ace/theme/twilight");
        editor.getSession().setMode("ace/mode/javascript");
        editor.$blockScrolling = Infinity;

        //load last editor contents from localstorage
        if (localStorage.hasOwnProperty("current_program"))
        {
            editor.setValue(localStorage.getItem("current_program"),1);
            $("#program_name").val(localStorage.getItem("current_program_name"));
        }

        //load led setings?
        if (localStorage.hasOwnProperty("settings_rows"))
        {
            rows=localStorage.getItem("settings_rows");
            cols=localStorage.getItem("settings_cols");
        }
        else
        {
            //get defaults
            rows=$("#ledsim").attr("height");
            cols=$("#ledsim").attr("width");
        }

        leds=rows*cols;
        $("#settings_rows").val(rows);
        $("#settings_cols").val(cols);
        $("#ledsim").attr("height", rows);
        $("#ledsim").attr("width", cols);


        update_quickload_list();
        compile_editor(editor);

        init_canvas();
        step();


        ////EVENT change led config
        $("#settings_update").on("click", function() {
            var cols=$("#settings_cols").val();
            var rows=$("#settings_rows").val();

            if (cols * rows > Module.MAX_LEDS)
                $("#settings_error").text("Total number of leds cannot be more than "+Module.MAX_LEDS);
            else if ( cols < 10 )
                $("#settings_error").text("Colums should be greater than 10");
            else if ( rows < 1 )
                $("#settings_error").text("Rows should be at least 1");
            else
            {
                localStorage.setItem("settings_rows", rows);
                localStorage.setItem("settings_cols", cols);
                document.location=document.location;

            }
        });

        ////EVENT when user changes the program, recompile and save it after a short delay
        editor.on("change", function() {
         $("#compiler_msg").html("&nbsp;");
         $("#compiler_msg").removeClass("error");
         clearTimeout(timeout);
         timeout = setTimeout(function() {
             compile_editor(editor);
        }, 300);
        });




        ///EVENT download button
        $("#download").click(function(){
            download($("#program_name").val()+".js", editor.getValue());
        });

        ///EVENT save button
        $("#save").click(function(){
            if ($("#program_name").val())
            {
                localStorage.setItem("program "+$("#program_name").val(), editor.getValue());
                update_quickload_list();
            }
        });


        ///EVENT quick load button
        $("body").on("click", ".quick_load", function()
        {
            editor.setValue(localStorage.getItem("program "+$(this).text()),1);
            $("#program_name").val($(this).text());

        });

        ///EVENT quick delete
        $("#delete").click(function()
        {
            localStorage.removeItem("program "+$("#program_name").val());
            update_quickload_list();
        });

    });

};
