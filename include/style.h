R"rawText(
/* Copyright 2014 Owen Versteeg; MIT licensed */body,textarea,input,select{background:0;border-radius:0;font:16px sans-serif;margin:0}.smooth{transition:all .2s}.btn,.nav a{text-decoration:none}.container{margin:0 20px;width:auto}label>*{display:inline}form>*{display:block;margin-bottom:10px}.btn{background:#999;border-radius:6px;border:0;color:#fff;cursor:pointer;display:inline-block;margin:2px 0;padding:12px 30px 14px}.btn:hover{background:#888}.btn:active,.btn:focus{background:#777}.btn-a{background:#0ae}.btn-a:hover{background:#09d}.btn-a:active,.btn-a:focus{background:#08b}.btn-b{background:#3c5}.btn-b:hover{background:#2b4}.btn-b:active,.btn-b:focus{background:#2a4}.btn-c{background:#d33}.btn-c:hover{background:#c22}.btn-c:active,.btn-c:focus{background:#b22}.btn-sm{border-radius:4px;padding:10px 14px 11px}.row{margin:1% 0;overflow:auto}.col{float:left}.table,.c12{width:100%}.c11{width:91.66%}.c10{width:83.33%}.c9{width:75%}.c8{width:66.66%}.c7{width:58.33%}.c6{width:50%}.c5{width:41.66%}.c4{width:33.33%}.c3{width:25%}.c2{width:16.66%}.c1{width:8.33%}h1{font-size:3em}.btn,h2{font-size:2em}.ico{font:33px Arial Unicode MS,Lucida Sans Unicode}.addon,.btn-sm,.nav,textarea,input,select{outline:0;font-size:14px}textarea,input,select{padding:8px;border:1px solid #ccc}textarea:focus,input:focus,select:focus{border-color:#5ab}textarea,input[type=text]{-webkit-appearance:none;width:13em}.addon{padding:8px 12px;box-shadow:0 0 0 1px #ccc}.nav,.nav .current,.nav a:hover{background:#000;color:#fff}.nav{height:24px;padding:11px 0 15px}.nav a{color:#aaa;padding-right:1em;position:relative;top:-1px}.nav .pagename{font-size:22px;top:1px}.btn.btn-close{background:#000;float:right;font-size:25px;margin:-54px 7px;display:none}@media(min-width:1310px){.container{margin:auto;width:1270px}}@media(max-width:870px){.row .col{width:100%}}@media(max-width:500px){.btn.btn-close{display:block}.nav{overflow:hidden}.pagename{margin-top:-11px}.nav:active,.nav:focus{height:auto}.nav div:before{background:#000;border-bottom:10px double;border-top:3px solid;content:'';float:right;height:4px;position:relative;right:3px;top:14px;width:20px}.nav a{padding:.5em 0;display:block;width:50%}}.table th,.table td{padding:.5em;text-align:left}.table tbody>:nth-child(2n-1){background:#ddd}.msg{padding:1.5em;background:#def;border-left:5px solid #59d}
/* Dark plugin */
html, body{
    background-color: #222222;
    color: #c9c9c9;
    height:100%;
}
input{
    background-color: #313131;
    color: #e4e4e4;
    border-color: #cccccc;
    border-style: solid;
}
label{
    white-space: nowrap;
    margin-top: 5px;
    display: block;
}
.col.c2{
    padding-top: 10px;
}

/* Min+ plugin*/
.section {
    display: flex;
    flex-flow: column;
    height: 90%;
}

.header {
    
}

.fullcontent {
    flex: 2;
    overflow: auto;
    background-color: #313131;
    border-color: #cccccc;
    border-width: 1px;
    border-style: solid;
    white-space: nowrap;
}

.footer {
    min-height: 60px;
}
	
.warning {
    border-color: #e44; 
    background: #fdd;
    color: #000;
}

.great {
    border-color: rgb(183, 207, 183);
    background: rgb(54, 54, 54);
    color: rgb(185, 185, 185);
}
)rawText"