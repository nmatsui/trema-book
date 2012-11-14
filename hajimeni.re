= はじめに

夕食前に自宅で原稿を書いていると、5歳になる長男が保育所で拾ってきた石を見せに来たものだ。見ると何の変哲もない灰色の石ころだが、本人にとっては大切なものだったようで、ぜったいに捨てないでね、と念を押してきた。そのうちこうした「たからもの」の石ころが机の上に5個、6個…とだんだんたまっていった。

思えば、この本もそんな石ころ集めから始まった。ベストセラー「プログラミングの心理学 - または、ハイテクノロジーの人間学(毎日コミュニケーションズ)」で有名なジェラルド・ワインバーグの最近の著書に「ワインバーグの文章読本 - 自然石構築法(翔泳社)」がある。これは、多作なワインバーグじきじきの書き方指南で、メモや引用といった文章の断片を「石」と呼び、この石を集めたり並べ替えたりすることで立派な石造りの建物やアーチを組み上げる、つまり一冊書き上げる方法を説明した名著だ。「よし、まずは石を集めるか」。自然石構築法を知った私は、さっそくこれに取りかかった。

本書の執筆にあたって集めた石は膨大なものになった。OpenFlowの仕様書や論文はもちろん、職場での雑談やソースコードの断片、大学での講義、メーリングリストへの投稿、ネットワーク業界のニュース、気晴らしの読書やテレビ、昔聴いていたラジオ番組、iPhoneに残っていた写真、とにかく大小いろんな色の石を、そこらじゅうから集めまくっては並べかえる毎日だった。

こうして集めた石の中には、けっして独力では収集できなかった貴重な石も多い。本書の原稿をgithubで公開してコメントを募集したころ@<fn>{github}、たくさんの方々から100件以上のレビューをいただいた。息子が勝手に机に石を置いていくように、こちらからお願いしなくとも善意で石を送ってくれる人も大勢いた。こうしたいくつかの石は、本書の重要な部分を占めている。とくに@<chap>{datacenter_wakame}のTremaを使ったデータセンター、@<chap>{google}のGoogleでのOpenFlowユースケース、そして@<chap>{diy_switch}のOpenFlowスイッチ自作法は、普通では絶対に手に入らない貴重な金ピカの宝石をいろいろな人からいただいたおかげで、何とか形にすることができた。@<br>{}

//footnote[github][@<href>{https://github.com/yasuhito/trema-book}]

「石集め」協力者のみなさん (敬称略、順不同): 坪井 俊樹、壬生 亮太、宮下 一博、石井 秀治、金海 好彦、@<tt>{@stereocat}、高田 将司、@<tt>{@SRCHACK}、富永 光俊、沼野 秀吾、黄 恵聖、富田 和伸、前川 峻志、園田 健太郎、大山 裕泰、藤原 智弘、空閑 洋平、佛崎 雅弘、阿部 博、小谷 大祐、笹生 健、山口 啓介、森部 正二朗、高橋 大輔、栄 純明、山崎 泰宏、高宮 友太郎、高宮 葵、高宮 優子。とくに、○○さんには、、、(特別感謝する人をここに書く)。@<br>{}

石はただ集めるだけではなく、必要な形に整形したり隙間を埋めたり、さらには磨き上げたりする必要がある。実は、私はもともとネットワークの専門家ではないので、ネットワークに特有な考え方や用語の説明に苦労した。そうした部分を補ってくれたのが、ネットワーク研究者でありこの本の共著者でもある鈴木一哉氏だった。また私なりにも、石1つひとつの正確さにはベストを尽くした。とある章で、酔っぱらいが三軒茶屋(東京都)から武蔵小杉(神奈川県)まで歩いて帰るというエピソードがあるが、本当に歩けるかどうか、私は実際に体を張って検証したのだった。

執筆と並行してやったのが、本書で取り上げたOpenFlowプログラミングフレームワークTremaの開発だ。Tremaはもともと、その場しのぎで書いたソフトウェアを出発として、大量のテストコード、リポジトリサーバーのクラッシュ、@<tt>{svn}から@<tt>{git}への乗り換え、二度の忘年会、いきなりの人事異動、インドとの長距離電話会議、を経験して鍛えられてきたフレームワークだ。ずいぶんと曲折を経たが、まさしく石のような意思で乗り切った。

私は、いいフレームワークといい本ができあがったと思う。私のいいか悪いかの判断基準は、「まだ誰もやっていないことは、いいことだ」という単純な考えに基いている。Tremaみたいなフレームワークは、まだ誰もやっていない。OpenFlowを正面きってここまで扱っている本は、まだ他にはない。だからどちらも私にとっては「いいもの」だ。もちろん、本当にいいか悪いかは、読者のみなさんのご判断におまかせする。

//flushright{
2012年11月14日 高宮 安仁 (@<tt>{@yasuhito})
//}