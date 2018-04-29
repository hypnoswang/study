set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'

" NerdTree
Plugin 'scrooloose/nerdtree'

" TagList
" Plugin 'vim-scripts/taglist'

" YouCompleteMe
"Plugin 'Valloric/YouCompleteMe'

" TagBar
Plugin 'majutsushi/tagbar'

" vim-go
Plugin 'fatih/vim-go'


" The following are examples of different formats supported.
" Keep Plugin commands between vundle#begin/end.
" plugin on GitHub repo
" Plugin 'tpope/vim-fugitive'
" plugin from http://vim-scripts.org/vim/scripts.html
" Plugin 'L9'
" Git plugin not hosted on GitHub
" Plugin 'git://git.wincent.com/command-t.git'
" git repos on your local machine (i.e. when working on your own plugin)
" Plugin 'file:///home/gmarik/path/to/plugin'
" The sparkup vim script is in a subdirectory of this repo called vim.
" Pass the path to set the runtimepath properly.
" Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}
" Install L9 and avoid a Naming conflict if you've already installed a
" different version somewhere else.
" Plugin 'ascenator/L9', {'name': 'newL9'}

" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
" To ignore plugin indent changes, instead use:
"filetype plugin on
"
" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to auto-approve removal
"
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line


"设置语法高亮
syntax enable
syntax on

"设置配色方案
"colorscheme torte

""可以在buffer的任何地方使用鼠标
"set mouse=a
set selection=exclusive
set selectmode=mouse,key

"高亮显示匹配的括号
set showmatch

"去掉vi一致性
"set nocompatible

"使用空格代替tab
"set expandtab

"设置缩进
set tabstop=4		"tab键的宽度	
set softtabstop=4
set shiftwidth=4
set autoindent
set cindent

set encoding=utf-8
"set fileencodings=utf-8,Unicode,ucs-bom,utf-8,cp936,gb18030,utf-16le,utf-16
"set fileencoding=gb18030
"set termencoding=utf-8

"窗口分割时,进行切换的按键热键需要连接两次,比如从下方窗口移动
"光标到上方窗口,需要<c-w><c-w>k,非常麻烦,现在重映射为<c-k>,切换的
"时候会变得非常方便.
nnoremap <C-h> <C-w>h
nnoremap <C-j> <C-w>j
nnoremap <C-k> <C-w>k
nnoremap <C-l> <C-w>l

" Python 文件的一般设置，比如不要 tab 等
autocmd FileType python set tabstop=4 shiftwidth=4 expandtab
autocmd FileType python map <F12> :!python %<CR>

"set tags=/home/hypnos/study/redis-4.0.6/tags
set tags=/home/hypnos/nginx-1.12.2/tags
set autochdir


"打开文件类型自动识别
filetype on

" for nerdtree
map nt :NERDTree<CR>
map ntc :NERDTreeClose<CR>
map ntt :NERDTreeToggle<CR>

"for tagbar
map to :TagbarOpen<CR>
map tt :TagbarToggle<CR>
map tc :TagbarClose<CR>

" high light the search
set hlsearch 

" for code folder
set fdm=syntax

" golang vim-go
let g:go_fold_enable = ['block', 'import', 'varconst', 'package_comment']
let g:tagbar_type_go = {  
			\ 'ctagstype' : 'go',  
			\ 'kinds'     : [  
			\ 'p:package',  
			\ 'i:imports:1',  
			\ 'c:constants',  
			\ 'v:variables',  
			\ 't:types',  
			\ 'n:interfaces',  
			\ 'w:fields',  
			\ 'e:embedded',  
			\ 'm:methods',  
			\ 'r:constructor',  
			\ 'f:functions'  
			\ ],  
			\ 'sro' : '.',  
			\ 'kind2scope' : {  
			\ 't' : 'ctype',  
			\ 'n' : 'ntype'  
			\ },  
			\ 'scope2kind' : {  
			\ 'ctype' : 't',  
			\ 'ntype' : 'n'  
			\ },  
			\ 'ctagsbin'  : 'gotags',  
			\ 'ctagsargs' : '-sort -silent'  
			\ }  

" for golang autocomplete gocode
imap <C-j> <C-x><C-o>
let g:go_def_mapping_enabled = 0
let g:go_list_autoclose = 1
" the time for auto closing the status window, such as autocompletion info,
" unit: ms
let g:go_statusline_duration = 1000
" disable to show the info of the func after an autocompletion
let g:go_echo_go_info = 0
autocmd FileType go map <C-]> :GoDef<CR>
autocmd FileType go map <C-t> :GoDefPop<CR>
