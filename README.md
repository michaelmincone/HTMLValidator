# HTMLValidator
Validates your HTML code!

The "sample pages" folder is filled with html files,
some of which are either valid or invalid (depending on the name).
The Validator function take in and std::string and returns a bool. 
THIS CODE WILL NOT VALIDATE ALL HTML, SO THESE ARE THE FOLLOWING RULES I TOOK INTO ACCOUNT FOR:

I have considered the tags:  html, head, body, title, div, p, br, span

The tags have the following restrictions on their contents/children: 
html – must have exactly 2 children: head and body.
The head element must come first. 
head – must have exactly 1 child: title. May not contain any other tags. 
body – may contain any number (0 or more) of the tags: div, p, br, span. May contain text. 
title – may only contain text. Must not be empty.  
div – may contain any number of the tags: div, p, br, span. May contain text.  
p – may contain any number of the tags: br, span. May contain text.  
br – Self closing tag. Contains no content.  
span – may only contain text. 

The first tag overall is of the form <!DOCTYPE html>  
The document is required to contain a valid html tag.

I have completed the definition of 'bool html_is_valid (const std::string& document)' where: 
The string 'document' contains the entire contents of a file to validate.  
Return true if the document contains valid html code as described above. 
Return false if any of the rules/specifications were violated. 

The following rules apply to the creation of tags: 

Tag names are not required to be lowercase.  
All tags must come as a pair of an opening tag <tagname> and a closing tag </tagname> unless the tag is self-closing.  
A tag may not close inside of any other tag that opened after it.  
If a tag is self-closing, the tag may appear as either <tagname> or <tagname/>  
A tag may contain an attribute called the id. This is a string that may not contain spaces and may not be empty. 
It would appear as <tagname id="id_value">. An id may not appear in a closing tag. 
  
The following rules apply to whitespace (tabs, new line, spaces) within a document: 
All whitespace in a document is treated as a space (i.e., the character ' ').  
All whitespace is omitted between the '>' symbol at the end of a tag (open or close), and the start of the next tag or content text.  
All strings of consecutive whitespace are treated as a single space. This happens both within a tag and within the content (e.g., 4 spaces to indent code in a document would be displayed as a single space on a page). 
In the descriptions above, text means any string (including the empty string, unless specified otherwise). Any number includes 0. 
1
