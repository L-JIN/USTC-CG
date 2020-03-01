<TeXmacs|1.99.11>

<style|<tuple|generic|chinese>>

<\body>
  <doc-data|<doc-title|USTC CG Lab 2 \U Image
  Warping>|<doc-author|<\author-data|<author-name|Jin Li>>
    \;
  </author-data>>|<doc-date|<date>>>

  <abstract-data|<abstract|\<#672C\>\<#6587\>\<#4E3B\>\<#8981\>\<#9610\>\<#8FF0\>\<#4E86\>IDW\<#548C\>RBF\<#4E24\>\<#79CD\>\<#56FE\>\<#50CF\>\<#7FD8\>\<#66F2\>\<#7B97\>\<#6CD5\>\<#53CA\>\<#5176\>\<#5B9E\>\<#73B0\>\<#8FC7\>\<#7A0B\>\<#FF0C\>\<#4F7F\>\<#7528\>cpp\<#914D\>\<#5408\>Qt\<#5B8C\>\<#6210\>\<#56FE\>\<#5F62\>\<#754C\>\<#9762\>\<#7684\>\<#6784\>\<#9020\>\<#3002\>>>

  <section|\<#5B9E\>\<#9A8C\>\<#5185\>\<#5BB9\>>

  \<#5B9E\>\<#73B0\> Image Warping \<#7684\>\<#4E24\>\<#79CD\>\<#7B97\>\<#6CD5\>\<#FF1A\>

  <\itemize>
    <item><hlink|Inverse distance-weighted interpolation method
    (IDW)|https://github.com/Ubpa/USTC_CG/blob/master/Homeworks/2_ImageWarping/documents/0_IDW.md>

    <item><hlink|Radial basis functions interpolation method
    (RBF)|https://github.com/Ubpa/USTC_CG/blob/master/Homeworks/2_ImageWarping/documents/1_RBF.md>
  </itemize>

  <section|\<#7B97\>\<#6CD5\>\<#63CF\>\<#8FF0\>>

  <subsection|IDW\<#7B97\>\<#6CD5\>>

  IDW\<#7B97\>\<#6CD5\>\<#4E3B\>\<#8981\>\<#6709\>\<#4EE5\>\<#4E0B\>\<#8981\>\<#70B9\>\<#FF1A\>

  <\itemize-dot>
    <item>\<#627E\>\<#5230\>\<#6EE1\>\<#8DB3\><math|\<b-f\><around|(|\<b-p\>|)>=<big|sum>w<rsub|i><around|(|\<b-p\>|)>*\<b-f\><rsub|i><around|(|\<b-p\>|)>>\<#7684\>\<#5C40\>\<#90E8\>\<#8FD1\>\<#4F3C\>\<#51FD\>\<#6570\>\<#FF1A\>

    <math|f<rsub|i><around*|(|p<rsub|i>|)>=q<rsub|i>>,
    \<#4E14\><math|<big|sum>w<rsub|i><around|(|\<b-p\>|)>=1>.

    <item>Shepard\<#63D0\>\<#51FA\>\<#6743\>\<#91CD\>\<#51FD\>\<#6570\><math|\<omega\><rsub|i><around|(|p|)>=<frac|\<sigma\><rsub|i><around|(|p|)>|<big|sum><rsub|j=1><rsup|n>\<sigma\><rsub|j><around|(|p|)>>>

    \<#5176\>\<#4E2D\>, <math|\<sigma\><rsub|j><around|(|p|)>=d<around*|(|p,p<rsub|j>|)><rsup|-\<mu\>>>\<#FF0C\>
    <math|d<around*|(|p,p<rsub|j>|)>>\<#662F\><math|p>\<#4E0E\><math|p<rsub|j>>\<#7684\>\<#8DDD\>\<#79BB\>\<#FF0C\><math|\<mu\>>\<#662F\>\<#5927\>\<#4E8E\>0\<#7684\>\<#4EFB\>\<#610F\>\<#6570\>
  </itemize-dot>

  \<#5728\>\<#672C\>\<#6B21\>\<#5B9E\>\<#9A8C\>\<#4E2D\>\<#FF0C\>\<#53D6\>\<#5C40\>\<#90E8\>\<#8FD1\>\<#4F3C\>\<#51FD\>\<#6570\>\<#4E3A\>\<#7EBF\>\<#6027\>\<#51FD\>\<#6570\>\<#FF0C\>\<#8003\>\<#8651\><math|f<rsub|i><around|(|p|)>=x<rsub|i><rprime|'>+<around|(|p-p<rsub|i>|)>*T<rsub|i>><with|font|roman|\<#FF0C\>>\<#8BEF\>\<#5DEE\>\<#51FD\>\<#6570\><math|E<around|(|T<rsub|i>|)>=<big|sum><rsub|j=1,j\<neq\>i><rsup|n>\<sigma\><rsub|i><around|(|p<rsub|j>|)>*<around|(|f<rsub|i><around|(|p<rsub|j>|)>-x<rsub|j><rprime|'>|)><rsup|2>>\<#3002\>

  \<#4E3A\>\<#4F7F\>\<#8BEF\>\<#5DEE\>\<#51FD\>\<#6570\>\<#5C3D\>\<#53EF\>\<#80FD\>\<#5C0F\><with|font|roman|\<#FF0C\>>\<#53EF\>\<#5BF9\><math|T<rsub|i>>\<#6C42\>\<#5BFC\>\<#6570\><with|font|roman|\<#FF0C\>>\<#5F97\>\<#5230\>\<#7EBF\>\<#6027\>\<#65B9\>\<#7A0B\>\<#7EC4\><with|font|roman|\<#FF0C\>>\<#4ECE\>\<#800C\>\<#901A\>\<#8FC7\>\<#89E3\>\<#7EBF\>\<#6027\>\<#65B9\>\<#7A0B\>\<#7EC4\><with|font|roman|\<#FF0C\>>\<#53EF\>\<#4EE5\>\<#89E3\>\<#5F97\><math|T<rsub|i>>.

  <subsection|RBF\<#7B97\>\<#6CD5\>>

  RBF\<#7ED9\>\<#51FA\>\<#7684\>\<#5750\>\<#6807\>\<#53D8\>\<#6362\>\<#516C\>\<#5F0F\>\<#4E3A\>

  <\equation*>
    f<around|(|p|)>=<big|sum><rsub|i=1><rsup|n>\<alpha\><rsub|i>*g<rsub|i><around|(|d<around|(|p<rsub|i>,p|)>|)>+A*p+B
  </equation*>

  <with|font|roman|\<#FF0C\>>\<#5176\>\<#4E2D\><math|g<rsub|i>>\<#4E3A\>\<#57FA\>\<#51FD\>\<#6570\>\<#FF0C\>\<#53D6\>

  <\equation*>
    g<rsub|i><around|(|d|)>=<around|(|d<rsup|2>+r<rsub|i><rsup|2>|)><rsup|<frac|1|2>>,r<rsub|i>=m*i*n<rsub|j\<neq\>i>*d<around|(|p<rsub|i>,p<rsub|j>|)>
  </equation*>

  \<#5176\>\<#4E2D\>\<#7684\>\<#7CFB\>\<#6570\><math|\<alpha\><rsub|i>>\<#53EF\>\<#4EE5\>\<#901A\>\<#8FC7\>\<#65B9\>\<#7A0B\>\<#7EC4\><math|f<around|(|p<rsub|i>|)>=q<rsub|i>>\<#89E3\>\<#5F97\>\<#3002\>

  <subsection|\<#767D\>\<#7F1D\>\<#586B\>\<#5145\>>

  \<#53D8\>\<#6362\>\<#8FC7\>\<#7A0B\>\<#5B58\>\<#5728\>\<#62C9\>\<#4F38\>\<#FF0C\>\<#4E14\>\<#50CF\>\<#7D20\>\<#70B9\>\<#662F\>\<#79BB\>\<#6563\>\<#7684\>\<#FF0C\>\<#6545\>\<#6709\>\<#4E9B\>\<#70B9\>\<#53EF\>\<#80FD\>\<#5E76\>\<#6CA1\>\<#6709\>\<#88AB\>\<#522B\>\<#7684\>\<#70B9\>\<#53D8\>\<#6362\>\<#5230\>\<#FF0C\>\<#9700\>\<#8981\>\<#8FDB\>\<#884C\>\<#767D\>\<#7F1D\>\<#586B\>\<#5145\>\<#3002\>

  \<#672C\>\<#6587\>\<#6240\>\<#91C7\>\<#7528\>\<#7684\>\<#586B\>\<#5145\>\<#65B9\>\<#6CD5\>\<#4E3A\>\<#FF1A\>\<#5728\>\<#7F1D\>\<#5468\>\<#56F4\>\<#4E00\>\<#56FA\>\<#5B9A\>\<#5927\>\<#5C0F\>\<#7684\>\<#77E9\>\<#9635\>\<#533A\>\<#57DF\>\<#4E2D\>\<#9009\>\<#62E9\>\<#4E0D\>\<#4E3A\>\<#767D\>\<#7F1D\>\<#7684\>\<#6765\>\<#586B\>\<#8865\>\<#FF08\>\<#63D2\>\<#503C\>\<#601D\>\<#8DEF\>\<#FF09\>\<#3002\>

  <section|\<#4EE3\>\<#7801\>\<#7B80\>\<#4ECB\>>

  \<#4E3B\>\<#8981\>\<#7684\>\<#7B97\>\<#6CD5\>\<#90E8\>\<#5206\>\<#5206\>\<#4E3A\>\<#56DB\>\<#4E2A\>\<#6587\>\<#4EF6\>\<#FF1A\><cpp|IDW.h,
  IDW.cpp, RBM.h, RBM.cpp>

  \<#5176\>\<#4E2D\>\<#5B9A\>\<#4E49\>\<#4E86\>\<#4E24\>\<#4E2A\>\<#7C7B\>\<#FF0C\>\<#5E76\>\<#6CA1\>\<#6709\>\<#518D\>\<#5EFA\>\<#4E00\>\<#4E2A\>\<#7236\>\<#7C7B\>\<#5C06\>\<#4E8C\>\<#8005\>\<#4F5C\>\<#4E3A\>\<#7EE7\>\<#627F\>\<#7C7B\>\<#FF0C\>\<#8FD9\>\<#662F\>\<#4E00\>\<#4E2A\>\<#4E0D\>\<#8DB3\>\<#7684\>\<#5730\>\<#65B9\>\<#3002\>

  <subsection|IDW>

  <\cpp-code>
    public:

    \ \ \ \ <code*|get_input_control_point_weight>
    \<#83B7\>\<#53D6\>\<#6BCF\>\<#4E2A\>\<#70B9\>\<#5BF9\>\<#5E94\>\<#63A7\>\<#5236\>\<#70B9\>\<#7684\>\<#6743\>\<#91CD\>\<#8868\>

    <code*| \ \ \ get_output_point> \<#83B7\>\<#53D6\>\<#8F93\>\<#5165\>\<#70B9\>\<#7684\>\<#53D8\>\<#6362\>\<#70B9\>

    <code*| \ \ \ get_image_IDW> \<#83B7\>\<#53D6\>\<#5904\>\<#7406\>\<#8FC7\>\<#7684\>\<#56FE\>\<#7247\>(\<#5305\>\<#542B\>\<#767D\>\<#7F1D\>\<#586B\>\<#5145\>)

    \ \ \ \ <code*|set_miu_> \<#8BBE\>\<#7F6E\>miu

    private

    \ \ \ \ double miu_
  </cpp-code>

  <subsection|RBF>

  \<#4F7F\>\<#7528\>Eigen\<#5E93\>\<#89E3\>\<#65B9\>\<#7A0B\>\<#3002\>

  <\cpp-code>
    public:

    \ \ \ \ <code*|cal_distance_martix> \<#8BA1\>\<#7B97\>\<#8DDD\>\<#79BB\>\<#77E9\>\<#9635\>

    <code*| \ \ \ calculate_ri> \<#8BA1\>\<#7B97\>p\<#4E2A\>\<#6570\>\<#636E\>\<#70B9\>\<#7684\>R\<#503C\>

    \ \ \ \ <code*|calculate_ai> \<#8BA1\>\<#7B97\>\<#65B9\>\<#7A0B\>\<#7EC4\>\<#7684\>ax,ay

    <code*| \ \ \ get_image_RBF> \<#83B7\>\<#53D6\>\<#5904\>\<#7406\>\<#8FC7\>\<#7684\>\<#56FE\>\<#7247\>(\<#5305\>\<#542B\>\<#767D\>\<#7F1D\>\<#586B\>\<#5145\>)

    private:

    \ \ \ \ _u, _d, _r, A_x, A_y
  </cpp-code>

  <subsection|Imagewidget>

  DoIDW\<#548C\>DoRBF\<#4E2D\>\<#5305\>\<#542B\>\<#4EA4\>\<#4E92\>\<#7684\>\<#903B\>\<#8F91\>\<#FF0C\>\<#4E3B\>\<#8981\>\<#5728\>\<#4E8E\>\<#8D77\>\<#70B9\>\<#548C\>\<#7EC8\>\<#70B9\>\<#7684\>\<#9009\>\<#53D6\>\<#3002\>

  <section|\<#603B\>\<#7ED3\>>

  \<#672C\>\<#6B21\>\<#5B9E\>\<#9A8C\>\<#5B9E\>\<#73B0\>\<#4E86\>\<#4E24\>\<#4E2A\>\<#7B97\>\<#6CD5\>\<#FF0C\>\<#65F6\>\<#95F4\>\<#6709\>\<#70B9\>\<#8D76\>\<#6240\>\<#4EE5\>\<#7ED3\>\<#6784\>\<#5E76\>\<#6CA1\>\<#6709\>\<#8BA9\>\<#81EA\>\<#5DF1\>\<#6EE1\>\<#610F\>\<#FF0C\>\<#540E\>\<#9762\>\<#4F1A\>\<#8FDB\>\<#884C\>\<#91CD\>\<#6784\>\<#3002\>\<#8FD9\>\<#6B21\>\<#89E3\>\<#51B3\>\<#4E86\>\<#4E0A\>\<#6B21\>\<#53CD\>\<#9988\>\<#4E2D\>\<#53D1\>\<#73B0\>\<#7684\>\<#5185\>\<#5B58\>\<#6CC4\>\<#6F0F\>\<#95EE\>\<#9898\>\<#FF0C\>\<#611F\>\<#89C9\>\<#6536\>\<#83B7\>\<#9887\>\<#4E30\>\<#3002\>\<#975E\>\<#5E38\>\<#611F\>\<#8C22\>\<#52A9\>\<#6559\>\<#7684\>\<#6587\>\<#6863\>\<#FF0C\>\<#6781\>\<#5C3D\>\<#8BE6\>\<#7EC6\>\<#FF0C\>\<#4F53\>\<#9A8C\>\<#975E\>\<#5E38\>\<#597D\>\<#3002\>
</body>

<initial|<\collection>
</collection>>

<\references>
  <\collection>
    <associate|auto-1|<tuple|1|?|..\\..\\..\\..\\AppData\\Roaming\\TeXmacs\\texts\\scratch\\no_name_6.tm>>
    <associate|auto-10|<tuple|4|?|..\\..\\..\\..\\AppData\\Roaming\\TeXmacs\\texts\\scratch\\no_name_6.tm>>
    <associate|auto-2|<tuple|2|?|..\\..\\..\\..\\AppData\\Roaming\\TeXmacs\\texts\\scratch\\no_name_6.tm>>
    <associate|auto-3|<tuple|2.1|?|..\\..\\..\\..\\AppData\\Roaming\\TeXmacs\\texts\\scratch\\no_name_6.tm>>
    <associate|auto-4|<tuple|2.2|?|..\\..\\..\\..\\AppData\\Roaming\\TeXmacs\\texts\\scratch\\no_name_6.tm>>
    <associate|auto-5|<tuple|2.3|?|..\\..\\..\\..\\AppData\\Roaming\\TeXmacs\\texts\\scratch\\no_name_6.tm>>
    <associate|auto-6|<tuple|3|?|..\\..\\..\\..\\AppData\\Roaming\\TeXmacs\\texts\\scratch\\no_name_6.tm>>
    <associate|auto-7|<tuple|3.1|?|..\\..\\..\\..\\AppData\\Roaming\\TeXmacs\\texts\\scratch\\no_name_6.tm>>
    <associate|auto-8|<tuple|3.2|?|..\\..\\..\\..\\AppData\\Roaming\\TeXmacs\\texts\\scratch\\no_name_6.tm>>
    <associate|auto-9|<tuple|3.3|?|..\\..\\..\\..\\AppData\\Roaming\\TeXmacs\\texts\\scratch\\no_name_6.tm>>
  </collection>
</references>

<\auxiliary>
  <\collection>
    <\associate|toc>
      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|1<space|2spc>\<#5B9E\>\<#9A8C\>\<#5185\>\<#5BB9\>>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-1><vspace|0.5fn>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|2<space|2spc>\<#7B97\>\<#6CD5\>\<#63CF\>\<#8FF0\>>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-2><vspace|0.5fn>

      <with|par-left|<quote|1tab>|2.1<space|2spc>IDW\<#7B97\>\<#6CD5\>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-3>>

      <with|par-left|<quote|1tab>|2.2<space|2spc>RBF\<#7B97\>\<#6CD5\>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-4>>

      <with|par-left|<quote|1tab>|2.3<space|2spc>\<#767D\>\<#7F1D\>\<#586B\>\<#5145\>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-5>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|3<space|2spc>\<#4EE3\>\<#7801\>\<#7B80\>\<#4ECB\>>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-6><vspace|0.5fn>

      <with|par-left|<quote|1tab>|3.1<space|2spc>IDW
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-7>>

      <with|par-left|<quote|1tab>|3.2<space|2spc>RBF
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-8>>

      <with|par-left|<quote|1tab>|3.3<space|2spc>Imagewidget
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-9>>
    </associate>
  </collection>
</auxiliary>