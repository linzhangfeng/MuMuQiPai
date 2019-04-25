<GameFile>
  <PropertyGroup Name="LoadingLayer_0" Type="Layer" ID="a4ba329e-8b2b-4e90-bf02-fd77656cc5a4" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" Tag="3" ctype="GameLayerObjectData">
        <Size X="1280.0000" Y="720.0000" />
        <Children>
          <AbstractNodeData Name="layout" CanEdit="False" ActionTag="-847462357" Tag="7" IconVisible="False" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1280.0000" Y="720.0000" />
            <Children>
              <AbstractNodeData Name="bg" CanEdit="False" ActionTag="2141767873" Tag="4" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" ctype="SpriteObjectData">
                <Size X="1280.0000" Y="720.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="640.0000" Y="360.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.0000" Y="1.0000" />
                <FileData Type="Normal" Path="StudioUI/LoadingUI/loading/LoadingBg.png" Plist="" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="loadingNode" ActionTag="664656705" Tag="28" IconVisible="True" LeftMargin="640.0000" RightMargin="640.0000" TopMargin="720.0000" ctype="SingleNodeObjectData">
                <Size X="0.0000" Y="0.0000" />
                <Children>
                  <AbstractNodeData Name="loading_icon" ActionTag="-8137724" Tag="14" IconVisible="False" LeftMargin="-110.0059" RightMargin="-109.9941" TopMargin="-535.8315" BottomMargin="315.8315" ctype="SpriteObjectData">
                    <Size X="220.0000" Y="220.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="-0.0059" Y="425.8315" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="StudioUI/LoadingUI/loading/loading_icon.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="loading_txt" ActionTag="-1918357931" Tag="13" IconVisible="False" LeftMargin="-74.5038" RightMargin="-74.4962" TopMargin="-276.7145" BottomMargin="236.7145" ctype="SpriteObjectData">
                    <Size X="149.0000" Y="40.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="-0.0038" Y="256.7145" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="StudioUI/LoadingUI/loading/loading_txt.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="640.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" />
                <PreSize X="0.0000" Y="0.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="loading_bar_bg" Visible="False" ActionTag="791246013" Tag="17" IconVisible="False" LeftMargin="225.0000" RightMargin="225.0000" TopMargin="483.8262" BottomMargin="178.1738" LeftEage="273" RightEage="273" TopEage="19" BottomEage="19" Scale9OriginX="273" Scale9OriginY="19" Scale9Width="284" Scale9Height="20" ctype="ImageViewObjectData">
                <Size X="830.0000" Y="58.0000" />
                <Children>
                  <AbstractNodeData Name="loading_bar_fg" ActionTag="544569699" Tag="19" IconVisible="False" LeftMargin="-0.4840" RightMargin="0.4840" TopMargin="-0.2513" BottomMargin="0.2513" ProgressInfo="100" ctype="LoadingBarObjectData">
                    <Size X="830.0000" Y="58.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="414.5160" Y="29.2513" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4994" Y="0.5043" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <ImageFileData Type="Normal" Path="StudioUI/LoadingUI/loading/loading_bar_fg.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="progressText" ActionTag="-1428932005" Tag="25" IconVisible="False" LeftMargin="15.5138" RightMargin="616.4862" TopMargin="49.0887" BottomMargin="-20.0887" FontSize="25" LabelText="游戏安装中(2/2)..." ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="198.0000" Y="29.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="15.5138" Y="-5.5887" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0187" Y="-0.0964" />
                    <PreSize X="0.2386" Y="0.5000" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="tip" ActionTag="623852123" Tag="26" IconVisible="False" LeftMargin="723.9713" RightMargin="21.0287" TopMargin="49.0890" BottomMargin="-20.0890" FontSize="25" LabelText="28.31%" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="85.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="808.9713" Y="-5.5890" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9747" Y="-0.0964" />
                    <PreSize X="0.1024" Y="0.5000" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="loading_game_icon" ActionTag="129673361" Tag="24" IconVisible="False" LeftMargin="285.5176" RightMargin="288.4824" TopMargin="-317.9443" BottomMargin="119.9443" ctype="SpriteObjectData">
                    <Size X="256.0000" Y="256.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="413.5176" Y="247.9443" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4982" Y="4.2749" />
                    <PreSize X="0.3084" Y="4.4138" />
                    <FileData Type="Normal" Path="StudioUI/LoadingUI/loading/icon256.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="640.0000" Y="207.1738" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2877" />
                <PreSize X="0.6484" Y="0.0806" />
                <FileData Type="Normal" Path="StudioUI/LoadingUI/loading/loading_bar_bg.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="640.0000" Y="360.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>