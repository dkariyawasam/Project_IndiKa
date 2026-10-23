#!/usr/bin/env python3
"""Compile and exercise production dialogue lookahead/line-advance helpers."""
from pathlib import Path
import re,subprocess,tempfile
root=Path(__file__).resolve().parents[1];source=(root/'src/text.c').read_text()
helpers=source[source.index('static u32 DialogueNextWordWidth'):source.index('u16 RenderText(struct TextPrinter *textPrinter)')]
header=r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32; typedef uint8_t bool8;
#define FALSE 0
#define TRUE 1
#define EOS 255
#define CHAR_SPACE 32
#define CHAR_NEWLINE 254
#define CHAR_PROMPT_CLEAR 251
#define CHAR_PROMPT_SCROLL 250
#define EXT_CTRL_CODE_BEGIN 252
#define CHAR_EXTRA_SYMBOL 249
#define CHAR_KEYPAD_ICON 248
#define PLACEHOLDER_BEGIN 253
#define WINDOW_HEIGHT 1
#define RENDER_STATE_SCROLL_START 3
#define RENDER_UPDATE 1
#define RENDER_REPEAT 2
struct TextPrinter {struct {u8 fontId,lineSpacing,currentY,currentX,x,windowId,letterSpacing;} printerTemplate;struct {struct {u8 glyphId;} sub;} subUnion;u8 state;};
struct {u8 maxLetterHeight;} gFonts[]={{14}};
int arrowResets=0;
u32 GetWindowAttribute(u8 id,u8 attr){return 4;}
void TextPrinterInitDownArrowCounters(struct TextPrinter *p){arrowResets++;}
u8 GetExtCtrlCodeLength(u8 code){return code==4?4:2;}
u32 GetStringWidth(u8 font,const u8 *p,u8 spacing){u32 n=0;while(*p!=EOS){if(*p==EXT_CTRL_CODE_BEGIN){p+=1+GetExtCtrlCodeLength(p[1]);continue;}if(*p==CHAR_EXTRA_SYMBOL||*p==CHAR_KEYPAD_ICON||*p==PLACEHOLDER_BEGIN)p++;n+=6;p++;}return n;}
'''
tests=r'''
int main(void){
 struct TextPrinter p={0};p.printerTemplate.lineSpacing=1;p.printerTemplate.currentY=1;p.printerTemplate.currentX=200;p.printerTemplate.x=0;
 const u8 plain[]={32,'w','o','r','d',32,'n','e','x','t',EOS};
 const u8 color[]={32,EXT_CTRL_CODE_BEGIN,4,32,254,250,'w','o','r','d',EOS};
 const u8 stop[]={32,'w',CHAR_PROMPT_CLEAR,'o',EOS};
 const u8 newline[]={32,'w',CHAR_NEWLINE,'o',EOS};
 const u8 symbol[]={32,CHAR_EXTRA_SYMBOL,32,'w',EOS};
 assert(DialogueNextWordWidth(&p,plain)==30);
 assert(DialogueNextWordWidth(&p,color)==30); // controls' space/newline bytes are operands
 assert(DialogueNextWordWidth(&p,stop)==12);
 assert(DialogueNextWordWidth(&p,newline)==12);
 assert(DialogueNextWordWidth(&p,symbol)==18);
 assert(DialogueNextLine(&p)==RENDER_REPEAT && p.printerTemplate.currentY==16 && p.printerTemplate.currentX==0);
 p.printerTemplate.currentX=200;
 assert(DialogueNextLine(&p)==RENDER_UPDATE && p.state==RENDER_STATE_SCROLL_START && arrowResets==1);
 assert(p.printerTemplate.currentY==16); // existing scroll machinery keeps the bottom line in place
 puts("PASS: word boundaries, encoded controls, explicit breaks, symbols and bottom-line scrolling");
}
'''
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);(p/'test.c').write_text(header+helpers+tests)
 subprocess.run(['cc','-std=c99',str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
