<TeXmacs|1.99.11>

<style|<tuple|generic|chinese>>

<\body>
  <doc-data|<doc-title|USTC CG \U Lab1: MiniDrawer>|<doc-author|<\author-data|<author-name|Jin
  Li>>
    \;
  </author-data>>|<doc-date|<date>>>

  <section|Intro>

  <subsection|MiniDrawer>

  \<#4F7F\>\<#7528\>Qt\<#914D\>\<#5408\>C++\<#5B9E\>\<#73B0\>\<#4E00\>\<#4E2A\>\<#753B\>\<#677F\>\<#FF0C\>\<#8981\>\<#6C42\>\<#7684\>\<#529F\>\<#80FD\>\<#6709\>\<#FF1A\>

  <\itemize>
    <item>\<#753B\>\<#76F4\>\<#7EBF\>\<#3001\>\<#957F\>\<#65B9\>\<#5F62\>\<#3001\>\<#692D\>\<#5706\>\<#3001\>\<#591A\>\<#8FB9\>\<#5F62\>\<#3001\>\<#968F\>\<#4FBF\>\<#753B\>

    <item>\<#6539\>\<#53D8\>\<#989C\>\<#8272\>\<#3001\>\<#6539\>\<#53D8\>\<#7C97\>\<#7EC6\>

    <item>Redo & Undo

    <item>\<#6253\>\<#5F00\>\<#56FE\>\<#7247\>\<#FF0C\>\<#5E76\>\<#5BF9\>\<#56FE\>\<#7247\>\<#8FDB\>\<#884C\>\<#66F4\>\<#6539\>
  </itemize>

  \<#540C\>\<#65F6\>\<#FF0C\>\<#8981\>\<#5BF9\>\<#7A0B\>\<#5E8F\>\<#8FDB\>\<#884C\>\<#5185\>\<#5B58\>\<#6CC4\>\<#6F0F\>\<#7684\>\<#68C0\>\<#67E5\>\<#3002\>

  <section|Implementation>

  <subsection|ViewWidget>

  \<#4F7F\>\<#7528\>ViewWidget\<#6765\>\<#63A5\>\<#53D7\>\<#4FE1\>\<#53F7\>\<#FF0C\>\<#4ECE\>\<#800C\>\<#5B9E\>\<#73B0\>\<#7528\>\<#6237\>\<#7684\>\<#52A8\>\<#4F5C\>\<#3002\>

  <cpp|public:>\<#5B9A\>\<#4E49\>\<#4E86\>\<#57FA\>\<#672C\>\<#54CD\>\<#5E94\>

  <\cpp>
    \ mousePressEvent, mouseMoveEvent, mouseReleaseEvent, paintEvent;
  </cpp>

  <cpp|slots:>\<#5B9A\>\<#4E49\>\<#4E86\>\<#4E00\>\<#7CFB\>\<#5217\><cpp|set>\<#65B9\>\<#6CD5\>\<#FF0C\>\<#53EF\>\<#4EE5\>\<#66F4\>\<#6539\>\<#529F\>\<#80FD\>\<#6216\>\<#66F4\>\<#6539\>\<#753B\>\<#7B14\>\<#5C5E\>\<#6027\>

  <cpp|private:>\<#4E3B\>\<#8981\>\<#5305\>\<#62EC\>\<#7528\>\<#6765\>\<#5B9E\>\<#73B0\>Redo\<#3001\>Undo\<#7684\>\<#6808\>\<#7ED3\>\<#6784\>\<#53CA\>\<#72B6\>\<#6001\>\<#5C5E\>\<#6027\>

  <subsection|Shape>

  \<#4F7F\>\<#7528\>Shape\<#6765\>\<#63A7\>\<#5236\>\<#4F5C\>\<#56FE\>\<#FF0C\>\<#4E3B\>\<#8981\>\<#5305\>\<#542B\>\<#8D77\>\<#6B62\>\<#70B9\>\<#7684\>\<#4FE1\>\<#606F\>\<#4E0E\><cpp|get>\<#65B9\>\<#6CD5\>\<#4E0E\>\<#7B14\>\<#5C5E\>\<#6027\>\<#FF0C\>\<#4EE5\>\<#53CA\>\<#51E0\>\<#4E2A\>\<#4F5C\>\<#56FE\>\<#6A21\>\<#5F0F\>\<#7684\>\<#7EE7\>\<#627F\>\<#7C7B\>\<#3002\>

  <subsection|MiniDraw>

  \<#4E3B\>\<#8981\>\<#7528\>\<#6765\>\<#663E\>\<#793A\>\<#83DC\>\<#5355\>\<#548C\>\<#63D0\>\<#793A\>\<#4FE1\>\<#606F\>\<#FF0C\>\<#5176\>\<#4E2D\>\<#4E5F\>\<#5305\>\<#542B\>\<#6539\>\<#53D8\>\<#7B14\>\<#6027\>\<#8D28\>\<#7684\>\<#4FE1\>\<#53F7\>\<#3002\>

  <subsection|Algorithms>

  <subsubsection|\<#591A\>\<#8FB9\>\<#5F62\>>

  \<#76F4\>\<#63A5\>\<#7EE7\>\<#627F\>Shape\<#FF0C\>\<#7136\>\<#540E\>\<#7528\>vector\<#5B58\>\<#9876\>\<#70B9\>\<#3002\>\<#9F20\>\<#6807\>\<#5DE6\>\<#952E\>\<#70B9\>\<#51FB\>\<#5F00\>\<#59CB\>\<#7ED8\>\<#56FE\>\<#FF0C\>\<#53F3\>\<#952E\>\<#70B9\>\<#51FB\>\<#4EE3\>\<#8868\>\<#7ED3\>\<#675F\>\<#3002\>

  <subsubsection|FreeHand>

  \<#76F4\>\<#63A5\>\<#628A\>\<#521D\>\<#59CB\>\<#70B9\>\<#52A0\>\<#5230\>shapelist\<#91CC\>\<#4F5C\>\<#4E3A\>\<#95F4\>\<#9694\>\<#3002\>\<#6BCF\>\<#6B21\>\<#79FB\>\<#52A8\>\<#4F5C\>\<#4E3A\>\<#7EBF\>\<#6BB5\>\<#52A0\>\<#5165\>shapelist

  \<#8FD9\>\<#91CC\>\<#7684\>shapelist\<#5C31\>\<#662F\>\<#7528\>\<#6765\>\<#5B9E\>\<#73B0\>Redo\<#548C\>Undo\<#7684\>\<#6808\>\<#7ED3\>\<#6784\>\<#3002\>

  <section|Conclusion>

  \<#8FD9\>\<#6B21\>\<#5B9E\>\<#9A8C\>\<#5B66\>\<#4E60\>\<#4E86\>Qt\<#7684\>\<#64CD\>\<#4F5C\>\<#FF0C\>\<#4E5F\>\<#7EC3\>\<#4E60\>\<#4E86\>C++\<#7684\>OOP\<#FF0C\>\<#6536\>\<#83B7\>\<#8F83\>\<#5927\>\<#3002\>\<#611F\>\<#8C22\>\<#52A9\>\<#6559\>\<#63D0\>\<#4F9B\>\<#8BE6\>\<#7EC6\>\<#7684\>\<#624B\>\<#518C\>\<#FF0C\>\<#53D7\>\<#76CA\>\<#532A\>\<#6D45\>\<#FF0C\>\<#7701\>\<#65F6\>\<#7701\>\<#529B\>\<#3002\>
</body>

<initial|<\collection>
</collection>>

<\references>
  <\collection>
    <associate|auto-1|<tuple|1|1>>
    <associate|auto-10|<tuple|3|1>>
    <associate|auto-2|<tuple|1.1|1>>
    <associate|auto-3|<tuple|2|1>>
    <associate|auto-4|<tuple|2.1|1>>
    <associate|auto-5|<tuple|2.2|1>>
    <associate|auto-6|<tuple|2.3|1>>
    <associate|auto-7|<tuple|2.4|1>>
    <associate|auto-8|<tuple|2.4.1|1>>
    <associate|auto-9|<tuple|2.4.2|1>>
  </collection>
</references>

<\auxiliary>
  <\collection>
    <\associate|toc>
      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|1<space|2spc>Intro>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-1><vspace|0.5fn>

      <with|par-left|<quote|1tab>|1.1<space|2spc>MiniDrawer
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-2>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|2<space|2spc>Implementation>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-3><vspace|0.5fn>

      <with|par-left|<quote|1tab>|2.1<space|2spc>ViewWidget
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-4>>

      <with|par-left|<quote|1tab>|2.2<space|2spc>Shape
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-5>>

      <with|par-left|<quote|1tab>|2.3<space|2spc>MiniDraw
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-6>>

      <with|par-left|<quote|1tab>|2.4<space|2spc>Algorithms
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-7>>

      <with|par-left|<quote|2tab>|2.4.1<space|2spc>\<#591A\>\<#8FB9\>\<#5F62\>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-8>>

      <with|par-left|<quote|2tab>|2.4.2<space|2spc>FreeHand
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-9>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|3<space|2spc>Conclusion>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-10><vspace|0.5fn>
    </associate>
  </collection>
</auxiliary>