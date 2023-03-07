# 42 minishell

## 概要

bashの再実装

- 多段パイプ (`|`)
- リダイレクト (`>`,`<`,`>>`)
- ヒアドキュメント (`<<`)
- $PATHにあるコマンド実行
- ビルトイン関数 (`echo`,`cd`,`pwd`,`export`,`unset`,`env`,`exit`)
- クォーテーション(`'`,`"`)
- 環境変数のセットや展開
- シグナルハンドル, EOF (`Ctrl+C`,`Ctrl+\`,`Ctrl+D`)
- 2人チーム

## Usage

`./minishell`

## キーワード

`C`,`構文解析`,`マルチプロセス`,`ファイルディスクリプタ`,`パイプ`,`リダイレクト`,`環境変数`,`シグナルハンドリング`
