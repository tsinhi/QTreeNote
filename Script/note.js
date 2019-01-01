function QTreeNoteHelp()
{
	
}

function getNoteHtml()
{
	try{
		return "<!DOCTYPE HTML>\n<html>\n" 
			+ document.head.outerHTML.replace(/<script.*<\/script>/g,"") + "\n"
			+ "<body class=\"ql-snow\">\n<div id=\"note-container\" class=\"ql-editor\">"
			+ window.noteQuill.container.firstChild.innerHTML + "</div>" +"\n</body>\n" +"\n</html>";
	}
	catch(e)
	{
		return "";
	}
}

function getNotePlainText()
{
	try{
		
		return document.getElementById('note-container').innerText;
	}
	catch(e)
	{
		return "";
	}
}

function initNoteEditor()
{
	if( document.head.innerHTML.search("<link rel=.*quill.snow.css") < 0 ){
		var linkScript = document.createElement("link");
		linkScript.rel = "stylesheet";
		linkScript.href= "../../Tools/quill/quill.snow.css";
		document.head.appendChild(linkScript)
	}

	var noteScript = document.createElement("script");
	noteScript.type = "text/javascript";
	noteScript.charset = "utf-8";
	noteScript.src="../../Tools/quill/quill.min.js";
	document.body.appendChild(noteScript);
	
	var noteScript1 = document.createElement("script");
	noteScript1.type = "text/javascript";
	noteScript1.charset = "utf-8";
	noteScript1.src="qrc:/Script/qwebchannel.js";
	document.body.appendChild(noteScript1);
	
}

function resizeEditorHeight()
{
	var editorHeight = document.body.clientHeight - document.getElementsByClassName("ql-toolbar")[0].offsetHeight;
	document.getElementById("note-container").style.height = editorHeight+"px";
	
}

function enableEditor()
{
	
	if(window.noteQuill){
		console.log("Quill  exist")
		return;
	}
		
	
	var toolbarOptions = [
		{ 'header': [1, 2, 3, 4, true] },
		{ 'font': [] },
		{ 'size': ['small', false, 'large', 'huge'] },  // custom dropdown
	  'bold', 'italic', 'underline', 'strike','blockquote', 'code-block',        // toggled buttons
	  { 'list': 'ordered'}, { 'list': 'bullet' },
	  { 'direction': 'rtl' },                         // text direction
	  'image',
	  
	  //[{ 'align': [] }],  // while input chinese ,the editor would crashed.
	  { 'color': [] }, { 'background': [] },          // dropdown with defaults from theme
	  'clean'                                        // remove formatting button
	];

	window.noteQuill = new Quill('#note-container', {
	  modules:{
		  toolbar:toolbarOptions
	  },
	  theme: 'snow'
	});
	
	var Image = Quill.import('formats/image');
	Image.sanitize = function(url) {
		return url; // You can modify the URL here
	};

	resizeEditorHeight();
	window.onresize = resizeEditorHeight;
	
}





function noteInsertHtml(htmlContent)
{
	const range = window.noteQuill.getSelection(true);
	
	window.noteQuill.pasteHTML(range.index,htmlContent);

 
}