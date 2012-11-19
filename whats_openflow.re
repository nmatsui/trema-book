= OpenFlowって何？

//lead{
ネットニュースをにぎわせているOpenFlowって何でしょうか？一体どんな仕組みで何がうれしいのでしょうか？本章を読むと、難しいネットワーク用語を知らなくても、身近なたとえ話ですっきりと理解できます。
//}

//indepimage[incredible_machine][][width=8cm]

== ソフトウェアで思いどおりにコントロールする

//quote{
無精(Laziness):エネルギーの総支出を減らすために、多大な努力をするように、あなたをかりたてる性質。こうして労力を省くために書いたプログラムは他人も使うようになり、そのプログラムに関する質問にいちいち答えずに済ますためにドキュメントを書くようになる。それゆえ、プログラマにとって最も重要な素質である。またそれゆえ、この本が存在するのである。(『プログラミングPerl』、Larry Wallら、オーム社)
//}

優れたプログラマが持つハッカー気質のひとつに「無精」があります。大好きなコンピュータの前から一時も離れずに、どうやってジャンクフードを手に入れるか…普通の人からするとただの横着に見えるかもしれませんが、ハッカー達にとってそれはいつでも大きな問題でした。

ソフトウェアによる横着はハッカーが最も創造性を発揮する分野の一つです。次の3つの有名な話は、どれもハッカーの横着文化と技術力の高さをよく表しています。

 1. ハッカーの巣窟として有名なMITのAIラボにはかつて、コンピュータからオンラインでピザを注文できるUNIXコマンドが存在しました@<fn>{xpizza}。ハックしていて腹が減ったらコマンドをたたいてピザを取る、なんとも横着です。
 2. コンピュータサイエンスの名門、カーネギーメロン大学にはコーク・マシンという変わったコーラ自販機があり、UNIXコマンド一発でコーラの冷え具合を確認できるようになっています@<fn>{coke_machine}。遠くの自販機まで行って@<bou>{ぬるい}コーラをつかまされないための工夫です。
 3. RFCで標準化されているコーヒーポットプロトコルでは、遠隔地にあるコーヒーポットのコーヒーの量を監視したり、コーヒーを自動的にいれたりするための半分冗談のインターフェースを定義しています@<fn>{rfc2324}。本当に実装してしまった人もいたそうですから驚きです。

//footnote[xpizza][MITの@<tt>{xpizza}コマンドのマニュアル:@<href>{http://stuff.mit.edu/afs/sipb/project/lnf/other/CONTRIB/ai-info}]
//footnote[coke_machine][カーネギーメロン大のコーク・マシンのサイト:@<href>{http://www.cs.cmu.edu/~coke/}]
//footnote[rfc2324][RFC 2324:@<href>{http://www.ietf.org/rfc/rfc2324.txt}]

こうした「ソフトウェアで楽をする」ハックのうち、もっとも大規模な例が最新鋭のデータセンターです。クラウドサービスの裏で動くデータセンターは極めて少人数のエンジニアが管理しており、大部分の管理作業をソフトウェアによって極限まで自動化している、という記事を読んだことがある人も多いでしょう。このようにピザやコーラ、コーヒーのようなお遊びから、データセンターのように一筋縄ではいかない相手まで、ソフトウェアで「モノ」を思いどおりにコントロールするのは何よりも楽しく、そして実際に役に立つハックの一種です。

== SDN:ネットワークをソフトウェアで制御しよう

その中でもネットワークをハックする技術の1つが、本書で取り上げるOpenFlowです。簡単に言えば、OpenFlowはネットワークスイッチの動作を自由にいじるためのAPIの一つです。スイッチひとつひとつの動作をAPIで書き換えられれば、究極的にはネットワーク全体を自由に制御できます。こうしたSoftware Defined Networking(SDN、ソフトウェアで定義されるネットワーク)を代表する野心的な技術がOpenFlowなのです。

OpenFlowの登場によって、これからはネットワークもプログラミングの対象になります。「いまだにネットワークを手で管理してるの？そんなのソフトウェアで横着できるだろ？」。今までは専門のオペレータ達が手作業で管理していたネットワークが、ついに無精なハッカー達の手にも渡るのです。

ハッカー達の創造性とOpenFlowのプログラマブルな特性が組み合わされば、次のような「究極の自動化」も夢ではなくなります。

 * 障害やトラフィック情報など、あらゆる情報を収集し集中管理できるネットワーク。
 * ユーザやアプリケーションの追加・削除に応じて、自動的に構成を変更するネットワーク。
 * 追加投資をしなくても、既存のインフラをめいっぱい使えるように最適化するネットワーク。

本書はこのすべてのトピックを扱います。自宅や職場のような中小規模ネットワークからデータセンターのような超大規模ネットワークまで、具体的なOpenFlowの適用例を見ながら「OpenFlowってどんなもので、具体的に何に使えるんだろう？」という素朴な疑問に答えます。また「さっそくOpenFlowを使ってすごいネットワークを作ってみたい!」というプログラマ向けには、実際に動かせる実用的なコードをたくさん載せました。

本書を読み進めるにあたって、ネットワークやプログラミングの深い知識は必要ありません。基本からひとつひとつ説明しますので、ネットワークの専門家はもちろん、プログラマやシステムエンジニア、そして営業職や管理職などなどOpenFlowに興味を持つ方であれば誰でもすんなり理解できるように構成してあります。ではさっそく、OpenFlowの仕組みを理解しましょう。

== OpenFlowの仕組みをたとえ話で

ここからちょっとした@<bou>{たとえ}話をします。みなさんもきっとお世話になったことがあると思いますが、よくあるカスタマーサポートを思い浮かべてください。そう、テレビとかパソコンの調子が悪くなったときに、フリーダイヤルで相談するアレです。それって、どこがOpenFlowと関係あるのでしょう？

実はOpenFlowの基本的な仕組みはカスタマーサポートにとても良く似ているのです。これからお話しする2つのストーリーが理解できれば、OpenFlowの95%を理解できたも同然です。それではさっそくこのストーリーの主人公の@<ruby>{友太郎,ゆうたろう}君と、カスタマーサポートセンターで働く青井さん、そして上司の宮坂主任の三人に登場してもらいましょう。

=== ストーリー1：エアコンが壊れた!サポートセンターに電話しよう

今年もエアコンの活躍する季節がやってきました。ところが友太郎君のエアコンはどうにも調子が良くありません。そこで取扱説明書に載っていたカスタマーサポートに電話することにしました。自動音声に従って問題ありそうな項目をすべてチェックしてみましたが、いっこうに解決しません。結局、自動音声はあきらめて電話オペレータに相談することになりました。

「はい、こちらカスタマーサポートセンターです。担当はわたくし青井がうけたまわります。ご用件は何でしょうか？」。青井さんはヨーヨーダイン・エアコン社で働く電話オペレータです。お客さんから不具合の症状を聞き出し、問題を解決できる適切なエンジニアに電話をつなぐのが仕事です(@<img>{yoyodyne_support})。

//image[yoyodyne_support][電話オペレータはお客さんからの問い合わせを適切なエンジニアへ転送][width=12cm]

友太郎君は不具合の様子を電話で伝えます。「なんだかリモコンの調子が悪いんです。温度表示がずっと点滅してるんですけど、どうしたら直りますか?」

青井さんは手元の対応マニュアルを調べます(@<table>{manual1})。対応マニュアルには故障の箇所と、それを直せるエンジニアの内線番号が書かれています。続く数字はそれぞれの問い合わせ件数です。

//table[manual1][電話オペレータ用対応マニュアル]{
故障の箇所			担当エンジニアの内線番号	問い合わせ件数
------------------------------------------------------------------
リモコン			555-2222					8件
エアコン本体		555-4444					6件
室外機				555-3333					4件
//}

マニュアルの先頭に探していた項目がみつかりました。「ご不便をおかけしました。担当のエンジニアにただいまおつなぎいたします」。電話を転送してもらった友太郎君はエンジニアと話をし、無事に問題を解決できました。

友太郎君との電話を切ると、青井さんは1番の問い合わせ件数を8件から9件にアップデートしました(@<table>{manual2})。

//table[manual2][問い合わせ件数をアップデートする]{
故障の箇所			担当エンジニアの内線番号	問い合わせ件数
------------------------------------------------------------------
リモコン			555-2222					@<em>{9件}
エアコン本体		555-4444					6件
室外機				555-3333					4件
//}

//noindent
こうすることでどんな症状の問い合わせが多いかを社内にフィードバックできます。たとえば問い合わせの傾向から、どこにエンジニアを集中して配置すればよいかが分かります。

==== これをOpenFlowに置換えると…

OpenFlowの世界では、パケットを送信するホストがお客さんの友太郎君、パケットを転送するOpenFlowスイッチが電話オペレータの青井さんに対応します(@<img>{openflow_host_switch})。ホストがパケットを送ると、OpenFlowスイッチはパケットの中身に応じてパケットを適切な宛先に転送します。これはちょうど、青井さんが友太郎君からの問い合わせに応じて担当エンジニアに電話を転送するのと同じです。

//image[openflow_host_switch][OpenFlowではホストがお客さん、スイッチが電話オペレータ、そしてフローテーブルがマニュアルに対応][width=12cm]

OpenFlowスイッチは、その動作が「マニュアル化」されています。カスタマーサポートの例では、青井さんはマニュアルを参照して転送先の番号を調べました。OpenFlow スイッチは、パケットの転送先をフローテーブルと呼ばれるデータベースを参照して決めます。青井さんの業務がすべてマニュアル化されているのと同じく、OpenFlow スイッチの制御はすべてこのフローテーブルの中身のみによって決まります。

==== 転送情報を管理するフローテーブル

フローテーブルには、「こういうパケットが届いたら、ポートx番に転送する」というルールがいくつか記録されています。このルールをフローエントリと呼びます。フローエントリはちょうどマニュアルの「リモコンの故障に関する問い合わせが来たら、内線555-2222に転送する」などといった各項目に対応します。

実際のフローテーブルの例を少し見てみましょう。@<table>{story1_openflow}はあるスイッチのフローテーブルで、各行がひとつひとつのフローエントリに対応します。フローエントリはマッチングルール、アクション、そして統計情報の3つの要素から構成されます。

//table[story1_openflow][フローテーブルとフローエントリの例]{
マッチングルール						アクション			統計情報
-------------------------------------------------------------------------------
送信元IPアドレスが192.168.1.100			ポート8番に転送		80パケット
宛先IPアドレスが192.168.10.92			ポート10番に転送	14パケット
送信元MACアドレスが00:50:56:c0:00:08	ポート1番に転送		24パケット
//}

: マッチングルール
  マッチングルールは届いたパケットをどう処理するかをフローテーブルから探す「キー」として使われます。たとえば「リモコンの調子がおかしい」という問い合わせから転送先の内線番号を決めたように、「パケットの送信元IPアドレスが192.168.1.100である」といったマッチングルールからパケットの転送先を決めます。

: アクション
  アクションは届いたパケットをどう扱うかという「対応内容」にあたります。たとえば「内線555-4444に転送」と同じく、アクションには「スイッチのポート8番に転送」などと指定します。なお、アクションでは単純な転送だけでなくパケットの書き換えもできます。

: 統計情報
  統計情報はフローエントリごとのパケット処理量の記録です。たとえば「リモコン関連の問い合わせ数は9件」とマニュアルに記録したように、「このフローエントリに従って転送したパケットは80個」などといった情報が書き込まれます。

いかがでしょうか？カスタマーサポートとOpenFlowは驚くほどよく似ていることがわかると思います。実はOpenFlowはとても単純で理解しやすい仕組みなのです。

=== ストーリー2：エアコンの新しい不具合を上司に問い合わせる

友太郎君のエアコンもしばらくは順調でしたが、一か月後また調子が悪くなってしまいました。友太郎君はふたたびカスタマーサポートへダイヤルします。「エアコンの排水ホースがすぐに詰まっちゃうんです」。

青井さんはいつものように手元の対応マニュアルを調べましたが、困ったことに排水ホースの項目は載っていません。どうやらまったく新しい不具合のようです。でも、青井さんは慌てずに友太郎君に次のように伝えます「すみませんが少々お待ちください。対応可能なエンジニアがいないかどうか問い合わせいたします」。そして電話口には録音された "しばらくお待ちください" のメッセージとどこか軽快な音楽が流れはじめました。

//image[yoyodyne_support_miyasaka][対応マニュアルに対処法が見つからなかった場合、上司に聞く][width=12cm]

こういう時、青井さんがいつも頼るのは上司の宮坂主任です(@<img>{yoyodyne_support_miyasaka})。「宮坂さん、排水ホースについての問い合わせが来てるんですが、誰につなげばいいかわかりますか?」宮坂主任はこの手のことはすべて頭の中に入っています。「それだったら山本君が適任だ」。そう言うと、内線番号を青井さんに渡してくれました。

転送先が分かった青井さんは友太郎君の待つ電話に戻ります。「大変お待たせいたしました。担当のエンジニアに転送いたします」。一度目の問い合わせと比べてかなり時間がかかってしまいましたが、これでようやく一件落着です。

さらに青井さんは、宮坂主任から教わったエンジニアの山本君の内線番号をマニュアルに追加しておきます(@<table>{manual3})。次からの同じ問い合わせにすばやく答えられるようにするためです。

//table[manual3][マニュアルに新しい症状と転送先を追加してアップデート]{
故障の箇所				担当エンジニアの内線番号		問い合わせ件数
----------------------------------------------------------------------
リモコン				555-2222						9件
エアコン本体			555-4444						6件
室外機					555-3333						4件
@<em>{排水ホース}		@<em>{555-5555}					@<em>{1件}
//}

==== これをOpenFlowに置換えると…

OpenFlowでこの上司にあたるのがコントローラと呼ばれるソフトウェアです(@<img>{openflow_host_switch_controller})。OpenFlowでネットワークをプログラミングする場合、プログラマが書くのはこのコントローラの部分です。頭脳であるコントローラをソフトウェアとして記述することで、ネットワークを自由自在に制御できるというわけです。

//image[openflow_host_switch_controller][フローテーブルにパケットのエントリが見つからなかった場合、コントローラに問い合わせる][width=12cm]

通常パケットはスイッチが高速に転送してくれますが、フローテーブルに載っておらずスイッチ側でどう処理してよいかわからない予期せぬパケットが届くこともあります。この場合スイッチはこのパケットをコントローラに上げて「このパケットどうしたらよいですか？」と指示をあおぎます。コントローラは問い合わせに応じてパケットの中身を調べ、どうすべきかという指示、つまりフローエントリをフローテーブルに書き込んでやります。

このようにフローテーブルに載っていないパケットが届くと、コントローラへの問い合わせが発生するのでパケット転送がとても遅くなります。しかし、スイッチの初期化時にコントローラが必要なフローエントリをあらかじめ書き込んでおくようにしておけば、スイッチ側だけで素早く処理できます。

=====[column] 友太郎の質問: コントローラへの問い合わせはどのくらい遅い？

Q. コントローラの問い合わせって実際どのくらい遅くなるの？フローテーブルを使わずに、毎回コントローラが指示を出せばいいんじゃない？

A. それをやると信じられないくらい遅くなります。試しにOpenFlowコントローラの開発フレームワークであるTrema(本書では全体を通じてTremaを使います) を使って簡単に実験してみたところ、ソフトウェアスイッチだけで転送する場合とすべてコントローラで処理する場合を比べると、性能が5倍も遅くなってしまいました。もちろんこれはいい加減な実験ですが、数倍は遅くなるという目安にはなります。また今回の実験ではソフトウェアスイッチを使ったのですが、ハードウェアスイッチを使うとこの差はさらに広がります。

=====[/column]

== OpenFlow のうれしさ

OpenFlow の仕組みの大枠は理解できたと思います。それでは最も肝心な部分、「OpenFlowって何がうれしいの？」を掘り下げてみましょう。

=== 自動化やシステム連携がしやすい

カスタマーサポートセンターでは、あらかじめ適切なマニュアルを作っておけば業務はすべて電話オペレータが自動的にやってくれます。全体の監督は管理職で実務は電話オペレータ、というふうにきっちりと分業できるからです。電話オペレータが実務をやってくれている間、管理職は他の部署との協業や調整に集中できます。

同様に、OpenFlow スイッチの制御はすべてソフトウェアであるコントローラで実現しているのでネットワーク管理の自動化が容易です。さらにコントローラが Ruby や Python、Java などよく知られた汎用言語で書いてあれば、既存のシステムやサービスなどとの連携も簡単です。さらに自動化とシステム連携を組み合わせれば、アプリケーションからの要求やビジネスポリシーの変更、問題発生などさまざまなトリガーに応じてネットワークの設定を変更するといった、一歩進んだ自動化もできます。

=== トラフィックエンジニアリングしやすい

カスタマーサポートセンターでは問い合わせ件数の情報はすべて管理職に上がってくるため、混み具合の把握や全体の交通整理が楽です。もし特定のエンジニアに問い合わせが集中しても、全体の状況はすべて管理職が把握しています。このため、問い合わせがうまくばらけるようにマニュアルを通じて電話オペレータの全員に指示できます。反対にもし各オペレータが個々に判断してしまうと、おなじエンジニアに問い合わせが偏ることは避けられません。

OpenFlow でもすべてのトラフィック情報はコントローラに上がってくるため、全体を見たトラフィックの最適化が可能です。各種統計情報を集計することで、コントローラはネットワーク全体のトラフィックデータを集められます。そしてその情報をもとに各スイッチのフローテーブルを更新することで、全体的に見て最適となるパケット転送経路を設定できます。反対にもし個々のスイッチが判断してしまうと、うまくトラフィックを分散できません。

=== ハードウェアが安くなる

カスタマーサポートセンターではとにかくたくさんの電話オペレータを雇うため、人件費をなるべく抑える工夫がされています。電話オペレータの業務をマニュアル化することで、特別な資格や実績がない人材でも短期間の研修で電話オペレータとして育てられます。このため、アルバイトやパート、派遣業務など幅広い範囲から比較的安く人材を集められます。

OpenFlow スイッチも従来のスイッチに比べると価格を低く抑えられます。OpenFlow では制御の主役が個々のスイッチからコントローラに移ります。このため、スイッチに求められる機能はフローテーブルの内容に従ってパケットを転送するだけ、というシンプルなものになります。このためスイッチの実装がシンプルになり価格が安くなります。加えて、コントローラを動かすマシンも汎用の Linux サーバーを利用することで、全体のハードウェア価格を低く抑えられます。

=== ベンダロックインが防げる

OpenFlow スイッチが安くなり、コモディティ化が進むとさまざまなベンダから OpenFlow スイッチが出てくる可能性があります。OpenFlow スイッチに求められる機能は OpenFlow プロトコルをしゃべれることだけです。このため、性能の差こそあれ OpenFlow に準拠していればどのベンダでのスイッチも同じコントローラで制御できます。これによって、特定のベンダのスイッチでないとダメといったいわゆるベンダロックインのリスクを避けられます。

また、ポート数密度や性能を気にしなければ、OpenFlow スイッチをソフトウェアで実現するという選択肢もあります。実際すでに Open vSwitch@<fn>{openvswitch} をはじめとしたさまざまなソフトウェア実装がそろっています。

//footnote[openvswitch][@<href>{http://openvswitch.org/}]

=== ソフトウェア開発のテクニックやツールが使える

コントローラはソフトウェアの一種なので、ソフトウェア開発で長年培われているさまざまなテクニックやツールをネットワークに応用できます。

 * コントローラのソースコードや関連データを git などのバージョン管理ツールで管理すれば、ネットワーク全体のバージョン管理やバージョン間の差分のチェック、および巻き戻しができるでしょう。
 * 近年主流のアジャイル開発手法でコントローラを開発すれば、反復的な機能追加が可能です。フィードバックを受けながらちょっとずつバージョンアップしていくことで、必要なネットワークを短期間で構築できます。
 * コントローラのユニットテストや受け入れテストを書くことで、ネットワーク全体を自動的にテストできます。テスト結果の出力は、そのまま仕様書の一部になります。もう Excel や Word で書いた仕様書を別個に管理する時代は終わりました。

== OpenFlow で気をつけなければいけないこと

もちろん、OpenFlow でもうれしいことばかりではありません。コントローラで制御を一手に引き受けるというモデルになっているため、スイッチの台数が増えたときのスケーラビリティに気を付ける必要があります。もし、フローテーブルに載っていないパケットが一気にコントローラへ到着すると、パケットの配送が遅延するか最悪の場合コントローラが死んでしまいます。

そこで、OpenFlow の使いどころやフローテーブルの残り容量には特に注意する必要があります。たとえば OpenFlow をインターネットのような多種多様のパケットが流れる環境につなげると、すぐにコントローラへの問い合わせが殺到しフローテーブルがいっぱいになって破綻してしまいます。しかしデータセンターなどの閉じた環境では、トラフィックの特徴や流れるパケットの種類はあらかじめ見当を付けておけます。そこで最低限のパケットのみがコントローラへ上がってくるようにうまくフローエントリとネットワークを設計することで、スイッチが増えてもスケールさせることが可能です。

== まとめ

本章では SDN を実現するための部品である OpenFlow を解説しました。OpenFlow はフローテーブルを持つスイッチと、フローテーブルの内容を集中管理するソフトウェアであるコントローラから成ります。ネットワーク管理をソフトウェア化することによって、自動化やさまざまなシステムとの連携、また汎用ハードウェアを使うことによるコストダウンなどなど種々の恩恵があります。

次章で少し詳しく OpenFlow の仕様を見たあと、残りの章ではこうしたトピックに 1 つひとつ触れていきます。
