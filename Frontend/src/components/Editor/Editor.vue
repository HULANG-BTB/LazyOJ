<template>
    <div class="editor">
        <div class="toobar">
            <el-form :inline="true">
                <el-form-item label="语言：">
                    <el-select v-model="language" placeholder="请选择" @change="languageChange">
                        <el-option
                                v-for="language in languages"
                                :label="language.title"
                                :value="language.lid">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item label="主题：">
                    <el-select v-model="theme" placeholder="请选择" @change="themeChange">
                        <el-option
                                v-for="theme in themes"
                                :label="theme"
                                :value="theme">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item label="字体：">
                    <el-select v-model="fontSize" placeholder="请选择" >
                        <el-option
                                v-for="theme in 24"
                                :label="theme*2"
                                :value="theme*2">
                        </el-option>
                    </el-select>
                </el-form-item>
            </el-form>
        </div>
        <div class="content">
            <codemirror ref="CMeditor" class="editor" :options="options" v-model="code" :style="{'font-size': fontSize + 'px'}"></codemirror>
        </div>
    </div>
</template>

<script>
    import { codemirror } from 'vue-codemirror'
    import 'codemirror/lib/codemirror.css'
    // import theme
    import 'codemirror/theme/3024-day.css'
    import 'codemirror/theme/3024-night.css'
    import 'codemirror/theme/abcdef.css'
    import 'codemirror/theme/ambiance-mobile.css'
    import 'codemirror/theme/ambiance.css'
    import 'codemirror/theme/base16-dark.css'
    import 'codemirror/theme/base16-light.css'
    import 'codemirror/theme/bespin.css'
    import 'codemirror/theme/blackboard.css'
    import 'codemirror/theme/cobalt.css'
    import 'codemirror/theme/colorforth.css'
    import 'codemirror/theme/darcula.css'
    import 'codemirror/theme/dracula.css'
    import 'codemirror/theme/duotone-dark.css'
    import 'codemirror/theme/duotone-light.css'
    import 'codemirror/theme/eclipse.css'
    import 'codemirror/theme/elegant.css'
    import 'codemirror/theme/erlang-dark.css'
    import 'codemirror/theme/gruvbox-dark.css'
    import 'codemirror/theme/hopscotch.css'
    import 'codemirror/theme/icecoder.css'
    import 'codemirror/theme/idea.css'
    import 'codemirror/theme/isotope.css'
    import 'codemirror/theme/lesser-dark.css'
    import 'codemirror/theme/liquibyte.css'
    import 'codemirror/theme/lucario.css'
    import 'codemirror/theme/material-darker.css'
    import 'codemirror/theme/material-ocean.css'
    import 'codemirror/theme/material-palenight.css'
    import 'codemirror/theme/material.css'
    import 'codemirror/theme/mbo.css'
    import 'codemirror/theme/mdn-like.css'
    import 'codemirror/theme/midnight.css'
    import 'codemirror/theme/monokai.css'
    import 'codemirror/theme/moxer.css'
    import 'codemirror/theme/neat.css'
    import 'codemirror/theme/neo.css'
    import 'codemirror/theme/night.css'
    import 'codemirror/theme/nord.css'
    import 'codemirror/theme/oceanic-next.css'
    import 'codemirror/theme/panda-syntax.css'
    import 'codemirror/theme/paraiso-dark.css'
    import 'codemirror/theme/paraiso-light.css'
    import 'codemirror/theme/pastel-on-dark.css'
    import 'codemirror/theme/railscasts.css'
    import 'codemirror/theme/rubyblue.css'
    import 'codemirror/theme/seti.css'
    import 'codemirror/theme/shadowfox.css'
    import 'codemirror/theme/solarized.css'
    import 'codemirror/theme/ssms.css'
    import 'codemirror/theme/the-matrix.css'
    import 'codemirror/theme/tomorrow-night-bright.css'
    import 'codemirror/theme/tomorrow-night-eighties.css'
    import 'codemirror/theme/ttcn.css'
    import 'codemirror/theme/twilight.css'
    import 'codemirror/theme/vibrant-ink.css'
    import 'codemirror/theme/xq-dark.css'
    import 'codemirror/theme/xq-light.css'
    import 'codemirror/theme/yeti.css'
    import 'codemirror/theme/yonce.css'
    import 'codemirror/theme/zenburn.css'

    import 'codemirror/theme/darcula.css'
    import 'codemirror/mode/python/python'
    // import language-support
    import 'codemirror/mode/clike/clike'
    import 'codemirror/mode/pascal/pascal'
    import 'codemirror/mode/ruby/ruby'
    import 'codemirror/mode/shell/shell'
    import 'codemirror/mode/python/python'
    import 'codemirror/mode/php/php'
    import 'codemirror/mode/perl/perl'
    import 'codemirror/mode/vbscript/vbscript'
    import 'codemirror/mode/scheme/scheme'
    import 'codemirror/mode/lua/lua'
    import 'codemirror/mode/javascript/javascript'
    import 'codemirror/mode/go/go'
    import 'codemirror/mode/sql/sql'
    import 'codemirror/mode/fortran/fortran'
    // import key-map
    import 'codemirror/keymap/sublime'

    export default {
        name: "Editor",
        data () {
            return {
                themes: [
                        '3024-day',
                        '3024-night',
                        'abcdef',
                        'ambiance-mobile',
                        'ambiance',
                        'base16-dark',
                        'base16-light',
                        'bespin',
                        'blackboard',
                        'cobalt',
                        'colorforth',
                        'darcula',
                        'dracula',
                        'duotone-dark',
                        'duotone-light',
                        'eclipse',
                        'elegant',
                        'erlang-dark',
                        'gruvbox-dark',
                        'hopscotch',
                        'icecoder',
                        'idea',
                        'isotope',
                        'lesser-dark',
                        'liquibyte',
                        'lucario',
                        'material-darker',
                        'material-ocean',
                        'material-palenight',
                        'material',
                        'mbo',
                        'mdn-like',
                        'midnight',
                        'monokai',
                        'moxer',
                        'neat',
                        'neo',
                        'night',
                        'nord',
                        'oceanic-next',
                        'panda-syntax',
                        'paraiso-dark',
                        'paraiso-light',
                        'pastel-on-dark',
                        'railscasts',
                        'rubyblue',
                        'seti',
                        'shadowfox',
                        'solarized',
                        'ssms',
                        'the-matrix',
                        'tomorrow-night-bright',
                        'tomorrow-night-eighties',
                        'ttcn',
                        'twilight',
                        'vibrant-ink',
                        'xq-dark',
                        'xq-light',
                        'yeti',
                        'yonce',
                        'zenburn',
                ],
                code: this.value.code,
                language: this.value.language,
                fontSize: 18,
                theme: '3024-day',
                mode: this.options.mode
            }
        },
        props: {
            value: {
                type: Object,
                default () {
                    return {
                        code: '',
                        language: ''
                    }
                }
            },
            options: {
                type: Object,
                default() {
                    return {
                        tabSize: 4,
                        mode: '',
                        lineNumbers: true,
                        matchBrackets: true,
                        keyMap: 'sublime',
                        theme: '3024-day'
                    }
                }
            },
            languages: {
                type: Array,
                default () {
                    return []
                }
            }
        },
        computed: {

        },
        methods: {
            languageChange (val) {
                const languageMap = {
                    1: 'text/x-csrc',
                    2: 'text/x-c++src',
                    3: 'text/x-pascal',
                    4: 'text/x-java',
                    5: 'text/x-ruby',
                    6: 'text/x-sh',
                    7: 'text/x-python',
                    8: 'application/x-httpd-php',
                    9: 'text/x-perl',
                    10: 'text/x-csharp',
                    11: 'text/x-objectivec',
                    12: 'text/vbscript',
                    13: 'text/x-scheme',
                    14: 'text/x-csrc',
                    15: 'text/x-c++src',
                    16: 'text/x-lua',
                    17: 'text/x-javascript',
                    18: 'text/x-go',
                    19: 'text/x-sql',
                    20: 'text/x-fortra',
                }
                this.mode = this.options.mode = languageMap[val-1]
                this.language = val
                this.update()
            },
            themeChange () {
                this.options.theme = this.theme
                this.$refs.CMeditor.codemirror.theme = this.theme
                this.update()
            },
            update () {
                this.$emit('input', {
                    code: this.code,
                    language: this.language
                })

            }
        },
        components: {
            codemirror
        },
        mounted() {
            let that = this
            that.$refs.CMeditor.codemirror.on('update', function () {
                that.update()
            })
            this.$refs.CMeditor.codemirror.setSize( 'auto', 500)
        }

    }
</script>

<style scoped>

</style>


